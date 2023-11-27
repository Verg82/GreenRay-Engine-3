/*******************************************************************************
// It's Unofficial Version Of The GreenRay Engine v.3.0.0.2
// GR_Physics.cpp
// It's The GreenRay Physics Engine.
// Made Specially For Implementing Physics And Collision Detection.
// Date Creation: 29 August 2005
// Last Changing: 21 January 2007
// Licensed By: (C)DDD Digital Dreams Development And (C)3D Studios Inc., 2007
*******************************************************************************/
#include <GR_Physics.h>

bool ObjectsColliding;

//Actor Globals
const NxCollisionGroup groupA = 1;
const NxU32 groupFlagA = 1 << groupA;

const NxCollisionGroup groupB = 2;
const NxU32 groupFlagB = 1 << groupB;

//Debugging Positions
CGRLabel* Lbl_PosX = NULL;
CGRLabel* Lbl_PosY = NULL;
CGRLabel* Lbl_PosZ = NULL;

CGRLabel* Lbl_Report = NULL; //Contact Report
CGRLabel* Lbl_Raycast = NULL; //Raycast Label
CGRLabel* Lbl_TubeReport = NULL; //Tube Report Label

CGRDecal* O_Decal[200];
int Num_Decals = 0;

CGRTube* gr_Tube[200]; //GreenRay Tubes Array
int Num_Tubes = 0;

CGRPhysObject* M_Barrel = NULL; //Barrel Model

//////////////////////////////////////////////////////////////////////////////
// Contact Report Virtual Class
//////////////////////////////////////////////////////////////////////////////
class ContactReport: public NxUserContactReport
{
  public:
    virtual void onContactNotify(NxContactPair& pair,NxU32 events)
	{
	  //This Is Where The Code To Respond To Collision Goes.
	  //You May Want To Set The PhysX Actor's Variable 'Name' 
	  //Using pair.actors[0]->setName("SomeName") To A Material Name 
	  //Such As 'Wood' Or 'Metal'. Then In This Callback, Use
	  //pair.actors[0]->getName() To Retrieve The Name And Play
	  //A Corresponding Sound.


      //Play Tube Down Sound 
      for (int I = 0; I < Num_Tubes; ++I)
      {
        if (gr_Tube[I] != NULL) 
	    { 
		  if (pair.actors[1]->getName() == "Loh")
		  {
		    if (gr_Tube[I]->Dropped == false) 
		    { 
			  //Show Tube Contact Report Label
              Lbl_TubeReport->FontColor = clRed;
              Lbl_TubeReport->Caption("Shell Contact Report: OK");
			  Lbl_TubeReport->Update();
              
			  //gr_Tube[I]->Rotation = 1.57f; //Set Good Rotation Position In Space   
			  gr_SoundsEff->PlaybackReset("TubeDown"); //Playback Tube Down From Sound Container 
			  //gr_Tube[I]->CreateShellSmoke();
              gr_Tube[I]->Dropped = true;

			  //gr_Tube[Num_Tubes-1]->Release(); 
			  //ReleaseClass(gr_Tube[Num_Tubes-1]); 
			  //DeleteClass(gr_Tube[Num_Tubes-1]);

              pair.actors[1]->setName(""); //Null Pair Name 
		    }
	      }
        }
	  }

	  ObjectsColliding = true;
    }
} gContactReport;

//////////////////////////////////////////////////////////////////////////////
// Raycast Report Virtual Class
//////////////////////////////////////////////////////////////////////////////
class RaycastReport: public NxUserRaycastReport
{
  public:
    virtual bool onHit(const NxRaycastHit& hit)
    {
      int userData = (int)hit.shape->getActor().userData;
      userData |= 1; //Mark As Hit
      hit.shape->getActor().userData = (void*)userData;

      const NxVec3 &worldImpact = hit.worldImpact;
	
	  return true;
    }
} gRaycastReport;

//////////////////////////////////////////////////////////////////////////////
// Construction/Destruction For CGRPhysics Class
//////////////////////////////////////////////////////////////////////////////
CGRPhysics::CGRPhysics(LPDIRECT3DDEVICE9 pDevice)
{
  d_pDevice = pDevice; //Apropritate Device Pointer To Class
  m_pPhysicsSDK = NULL; 
  m_pScene = NULL; 
  m_pPhysXPlayer = NULL;
  ObjectsColliding = false;

  //Initialize PhysicsSDK
  m_pPhysicsSDK = NxCreatePhysicsSDK(NX_PHYSICS_SDK_VERSION,NULL,NULL);
  //m_pPhysicsSDK->setParameter(NX_CCD_EPSILON,0.01f);
  //m_pPhysicsSDK->setParameter(NX_CONTINUOUS_CD,1);
  //m_pPhysicsSDK->setParameter(NX_SKIN_WIDTH, 0.025f);
  m_pPhysicsSDK->setParameter(NX_SKIN_WIDTH, 0.001f);

  //Collide 
  m_pPhysicsSDK->setParameter(NX_VISUALIZATION_SCALE, 1);
  m_pPhysicsSDK->setParameter(NX_VISUALIZE_COLLISION_SHAPES, 1);
  m_pPhysicsSDK->setParameter(NX_VISUALIZE_ACTOR_AXES, 1);
  m_pPhysicsSDK->setParameter(NX_VISUALIZE_CONTACT_POINT, 1);
  m_pPhysicsSDK->setParameter(NX_VISUALIZE_CONTACT_NORMAL, 1); 

  //Create A PhysX Scene
  NxSceneDesc Scene_Desc;
  Scene_Desc.gravity = NxVec3(0.0f,-40.0f,0.0f);
  Scene_Desc.userContactReport = &gContactReport;

  m_pScene = m_pPhysicsSDK->createScene(Scene_Desc);

  //Set Default Material
  NxMaterialDesc Material;
  Material.restitution	   = 0.0f;
  Material.staticFriction  = 0.5f;
  Material.dynamicFriction = 0.5f;
  DefaultMaterial = 0;
  //We Redefine The Default Material In Slot 0
  m_pScene->getMaterialFromIndex(DefaultMaterial)->loadFromDesc(Material); 
	
  //Set Frictionless Material For Player And Walls
  Material.restitution	   = 0.0f;
  Material.staticFriction  = 0.0f;
  Material.dynamicFriction = 0.0f;
  FrictionlessMaterial = m_pScene->createMaterial(Material)->getMaterialIndex();

  //Create The Player Object
  //We Use A Box To Represent The Player

  //Define Physics Object
  NxBodyDesc Body_Desc;
  Body_Desc.angularDamping = 0.5f;
  Body_Desc.linearVelocity = NxVec3(0.0f, 0.0f, 0.0f);

  //NxCapsuleShapeDesc Capsule_Desc;
  //Capsule_Desc.height = 2.0f;
  //Capsule_Desc.radius = 2.0f;
        
  NxBoxShapeDesc Box_Desc; 
  Box_Desc.dimensions = NxVec3(2.0f, 2.0f, 2.0f); //Player Collidable Box Size

  //Make Sure The Walls And Player All Use A Frictionless Material
  Box_Desc.materialIndex = FrictionlessMaterial;
  //Capsule_Desc.skinWidth = 0.0001f;

  NxActorDesc Actor_Desc;
  Actor_Desc.shapes.pushBack(&Box_Desc);
  Actor_Desc.body = &Body_Desc;
  Actor_Desc.density = 10.0f;
  Actor_Desc.globalPose.t = NxVec3(0.0f, 50.0f, 10.0f);

  //Add New Capsule Object To Physics Scene
  m_pPhysXPlayer = m_pScene->createActor(Actor_Desc);


  //Create GreenRay Debug Lbl_NormalX Label 
  Lbl_PosX = new CGRLabel(pDevice); //Construct A FPS Label
  Lbl_PosX->FontColor = clBlue; //Set Font Color
  Lbl_PosX->FontName = "MS Sans Serif"; //Set Font Name
  Lbl_PosX->FontSize = 10; //Set Font Size
  Lbl_PosX->FontStyle = FS_BOLD; //Set Font Style
  Lbl_PosX->Left = 738;
  Lbl_PosX->Top = 89;
  Lbl_PosX->Update(); //Update Label Parameters

  //Create GreenRay Debug Lbl_NormalY Label
  Lbl_PosY = new CGRLabel(pDevice); //Construct A FPS Label
  Lbl_PosY->FontColor = clBlue; //Set Font Color
  Lbl_PosY->FontName = "MS Sans Serif"; //Set Font Name
  Lbl_PosY->FontSize = 10; //Set Font Size
  Lbl_PosY->FontStyle = FS_BOLD; //Set Font Style
  Lbl_PosY->Left = 738;
  Lbl_PosY->Top = 103;
  Lbl_PosY->Update(); //Update Label Parameters

  //Create GreenRay Debug Lbl_NormalZ Label
  Lbl_PosZ = new CGRLabel(pDevice); //Construct A FPS Label
  Lbl_PosZ->FontColor = clBlue; //Set Font Color
  Lbl_PosZ->FontName = "MS Sans Serif"; //Set Font Name
  Lbl_PosZ->FontSize = 10; //Set Font Size
  Lbl_PosZ->FontStyle = FS_BOLD; //Set Font Style
  Lbl_PosZ->Left = 738;
  Lbl_PosZ->Top = 117;
  Lbl_PosZ->Update(); //Update Label Parameters

  //Create GreenRay Debug Lbl_Report Contact Report Label
  Lbl_Report = new CGRLabel(pDevice); //Construct A Label
  Lbl_Report->FontColor = clWhite; //Set Font Color
  Lbl_Report->FontName = "MS Sans Serif"; //Set Font Name
  Lbl_Report->FontSize = 10; //Set Font Size
  Lbl_Report->FontStyle = 0; //Set Font Style
  Lbl_Report->Left = 738;
  Lbl_Report->Top = 52;
  Lbl_Report->Update(); //Update Label Parameters

  //Create Raycast Label 
  Lbl_Raycast = new CGRLabel(pDevice); //Construct A Raycast Label
  Lbl_Raycast->FontColor = clWhite; //Set Font Color
  Lbl_Raycast->FontName = "MS Sans Serif"; //Set Font Name
  Lbl_Raycast->FontSize = 10; //Set Font Size
  Lbl_Raycast->FontStyle = 0; //Set Font Style
  Lbl_Raycast->Left = 738;
  Lbl_Raycast->Top = 67;
  Lbl_Raycast->Update(); //Update Label Parameters

  //Create GreenRay Debug Lbl_TubeReport Contact Report Label
  Lbl_TubeReport = new CGRLabel(pDevice); //Construct A  Label
  Lbl_TubeReport->FontColor = clWhite; //Set Font Color
  Lbl_TubeReport->FontName = "MS Sans Serif"; //Set Font Name
  Lbl_TubeReport->FontSize = 10; //Set Font Size
  Lbl_TubeReport->FontStyle = 0; //Set Font Style
  Lbl_TubeReport->Left = 738;
  Lbl_TubeReport->Top = 138;
  Lbl_TubeReport->Update(); //Update Label Parameters

  //Create Barrel PhysX Model Here:
  M_Barrel = new CGRPhysObject(d_pDevice,m_pScene);
  M_Barrel->SetTexturePath("\Textures\\Barrel\\Barrel01\\");
  M_Barrel->Create(L"\Models\\Barrel\\Barrel01\\Barrel.x");

  //Generate Triangles Adjacency For Physics Collision
  //TrianglesFromDXMeshVerb(M_Barrel->GetMesh(),M_Barrel->GetActor(),NxVec3(0,0.001f,0));

  //d_pPhysX->Init(GRPHYSX_BARREL);
} //EndConstructionDirectives

CGRPhysics::~CGRPhysics()
{
} //EndDestructionDirectives

//-----------------------------------------------------------------------------
// Name: CGRPhysics::Release()
// Desc: 
//-----------------------------------------------------------------------------
void CGRPhysics::Release()
{
  //Destroy The Physics
  if (m_pPhysicsSDK != NULL)
  {
    if (m_pScene != NULL) m_pPhysicsSDK->releaseScene(*m_pScene);
    m_pPhysicsSDK->release();
  }
} //EndReleaseProcedure

//-----------------------------------------------------------------------------
// Name: CGRPhysics::NxVec3_To_DXVec3()
// Desc: Convert a PhysX vec3 to a DX vec3
//-----------------------------------------------------------------------------
D3DXVECTOR3 CGRPhysics::NxVec3_To_DXVec3(NxVec3 PhysXVec3)
{
  return D3DXVECTOR3(PhysXVec3.x,PhysXVec3.y,PhysXVec3.z);
} //EndNxVec3_To_DXVec3Function

//-----------------------------------------------------------------------------
// Name: CGRPhysics::BeginUpdatePhysX()
// Desc: 
//-----------------------------------------------------------------------------
void CGRPhysics::BeginUpdatePhysX()
{
  //Update The Physics Before Rendering
  m_pScene->simulate(1.0f/60.0f);
} //EndBeginUpdatePhysXProcedure

//-----------------------------------------------------------------------------
// Name: CGRPhysics::EndUpdatePhysX()
// Desc: 
//-----------------------------------------------------------------------------
void CGRPhysics::EndUpdatePhysX()
{
  //Reset To False
  ObjectsColliding = false;

  //Fetch Simulation Results
  m_pScene->flushStream();
  m_pScene->fetchResults(NX_RIGID_BODY_FINISHED,true);
} //EndEndUpdatePhysXProcedure

//-----------------------------------------------------------------------------
// Name: CGRPhysics::UpdatePhysXPlayer()
// Desc: 
//-----------------------------------------------------------------------------
void CGRPhysics::UpdatePhysXPlayer()
{
  //Create Matrix To Represent Rotation And Set To Identity
  NxMat33 NewPlayerMat;
  NewPlayerMat.id();

  //Zero The Rotation Of The Player
  m_pPhysXPlayer->setGlobalOrientation(NewPlayerMat);

  //Set The X/Z Velocity And Let The Physics Handle The Y Velocity
  NxVec3 PlayerVel;
  PlayerVel = m_pPhysXPlayer->getLinearVelocity();
  PlayerVel.x = gr_Camera->Velocity.x*50.0f;
  PlayerVel.z = gr_Camera->Velocity.z*50.0f;
  m_pPhysXPlayer->setLinearVelocity(PlayerVel);

  //Set The Camera Position Based On The Player Physics Object
  gr_Camera->Position = NxVec3_To_DXVec3(m_pPhysXPlayer->getGlobalPosition());
  gr_Camera->Target = gr_Camera->Position+gr_Camera->Look_Vector*20.0f;

  //Set The Eye Position To Be Even With The Top Of The Physics Object And Player Height
  gr_Camera->Position.y+= 1.8f;
  gr_Camera->Target.y+= 1.8f;

  D3DXMatrixLookAtLH(&gr_Camera->View,&gr_Camera->Position,&gr_Camera->Target,&gr_Camera->Up);

  //Update The Active View
  d_pDevice->SetTransform(D3DTS_VIEW,&gr_Camera->View);
} //EndUpdatePhysXPlayerProcedure

//-----------------------------------------------------------------------------
// Name: CGRPhysics::JumpPhysXPlayer()
// Desc: 
//-----------------------------------------------------------------------------
void CGRPhysics::JumpPhysXPlayer(float Jump_Speed)
{
  //Make The Player Jump
  m_pPhysXPlayer->setLinearVelocity(NxVec3(0.0f,Jump_Speed,0.0f));
} //EndJumpPhysXPlayerProcedure

//-----------------------------------------------------------------------------
// Name: CGRPhysics::GenerateTrianglesFromDXMesh()
// Desc: Generate A Convex Physics Object From A DX Mesh
//-----------------------------------------------------------------------------
void CGRPhysics::GenerateTrianglesFromDXMesh(ID3DXMesh* &m_pMesh,
NxActor* &PhysXActor,NxVec3 &Mesh_Pos)
{
  //Used To Retrieve Flexible Vertex Format Information From Mesh File
  typedef struct 
  {
    D3DXVECTOR3 VertexPos;
	D3DXVECTOR3 Normal;
	D3DXVECTOR2 TexCoord;
  } MESH_FVF;

  //Used To Copy Indices
  typedef struct 
  {
    short IBNumber[3];
  } IndexBufferStruct;

  int Num_Vertices = m_pMesh->GetNumVertices();
  int Num_Triangles = m_pMesh->GetNumFaces();

  DWORD FVF_Size = D3DXGetFVFVertexSize(m_pMesh->GetFVF());

  //Create Pointer For Vertices
  Vertices = new NxVec3[Num_Vertices];

  //Get Vertex Buffer Of Mesh
  char* DXMeshPtr;
  m_pMesh->LockVertexBuffer(D3DLOCK_READONLY,(void**)&DXMeshPtr);
  for (int I = 0; I < Num_Vertices; I++)
  {
    MESH_FVF* DXMeshFVF = (MESH_FVF*)DXMeshPtr;
	Vertices[I] = NxVec3(DXMeshFVF->VertexPos.x,DXMeshFVF->VertexPos.y,DXMeshFVF->VertexPos.z);
	DXMeshPtr+= FVF_Size;
  }
  m_pMesh->UnlockVertexBuffer();

  //Get Index Buffer Of Mesh
  DWORD* Ind_Buff = 0;
  Triangles = 0;
  Triangles = new DWORD[Num_Triangles*3]; 
  m_pMesh->LockIndexBuffer(D3DLOCK_READONLY,(void**)&Ind_Buff); 
  memcpy(Triangles,Ind_Buff,sizeof(DWORD)*Num_Triangles*3); 
  m_pMesh->UnlockIndexBuffer();

  //Create Pointer For Indices
  /*IndexBufferStruct* Triangles = new IndexBufferStruct[Num_Triangles];

  //Get Index Buffer Of Mesh
  IndexBufferStruct* DXMeshIBPtr;
  m_pMesh->LockIndexBuffer(D3DLOCK_READONLY,(void**)&DXMeshIBPtr);
  for (int I = 0; I < Num_Triangles; I++)
  {
	Triangles[I].IBNumber[0] = DXMeshIBPtr[I].IBNumber[0];
	Triangles[I].IBNumber[1] = DXMeshIBPtr[I].IBNumber[1];
	Triangles[I].IBNumber[2] = DXMeshIBPtr[I].IBNumber[2];
  }
  m_pMesh->UnlockIndexBuffer();*/

  //Material Description For This Mesh
  NxMaterialDesc X_Material;

  //Frictionless Material For Player And Walls
  X_Material.restitution	 = 0.0f;
  X_Material.staticFriction	 = 0.0f;
  X_Material.dynamicFriction = 0.0f;
  int FrictionlessMaterial = m_pScene->createMaterial(X_Material)->getMaterialIndex();

  //Create Descriptor For Mesh
  NxTriangleMeshDesc Mesh_Desc;
  Mesh_Desc.numVertices		    = Num_Vertices;
  Mesh_Desc.numTriangles		= Num_Triangles;
  Mesh_Desc.pointStrideBytes	= sizeof(NxVec3);
  Mesh_Desc.triangleStrideBytes = 3*sizeof(NxU16);
  Mesh_Desc.points			    = Vertices;
  Mesh_Desc.triangles		    = Triangles;
  Mesh_Desc.flags				= NX_MF_16_BIT_INDICES;
  //Mesh_Desc.heightFieldVerticalAxis = NX_Y; 
  //Mesh_Desc.heightFieldVerticalExtent = -100.0f; 

  //Initialize Cooking
  bool Be_Cook = InitCooking();
  if (!Be_Cook) ::MessageBox(NULL,"GR_Physics.cpp: Unable To Initialize The Cooking Library.",NULL,NULL);

  //Cooking From Memory
  MemoryWriteBuffer Mem_Buf;
  if (CookTriangleMesh(Mesh_Desc,Mem_Buf))
  { 
    //Mesh Shape Description
    NxTriangleMeshShapeDesc Mesh_ShapeDesc;
	Mesh_ShapeDesc.meshData = m_pPhysicsSDK->createTriangleMesh(MemoryReadBuffer(Mem_Buf.data));

	//For Load From File
    //bool status = NxCookTriangleMesh(TriMeshDesc,UserStream("c:\\tmp.bin",false)); 
    //Mesh_ShapeDesc.meshData = m_pPhysicsSDK->createTriangleMesh(UserStream("c:\\tmp.bin",true)); 

	//Mesh_ShapeDesc.shapeFlags = NX_SF_FEATURE_INDICES;
	Mesh_ShapeDesc.materialIndex = FrictionlessMaterial;
    //Mesh_ShapeDesc.group = 1;
  
    if (Mesh_ShapeDesc.meshData)
	{
	  //Create Body
	  NxBodyDesc Body_Desc;
	  Body_Desc.angularDamping = 0.0f;
	  Body_Desc.linearVelocity = NxVec3(0.0f,0.0f,0.0f);
  
      //Body Description And Parameters
	  NxActorDesc Actor_Desc;
	  Actor_Desc.shapes.pushBack(&Mesh_ShapeDesc);
	  Actor_Desc.body		  = &Body_Desc;
	  Actor_Desc.density	  = 10.0f;      //Mass Of Object
	  Actor_Desc.globalPose.t = Mesh_Pos;   //Position In Space

	  PhysXActor = m_pScene->createActor(Actor_Desc);  
	  PhysXActor->userData = (void*)1;
    }
  }

  CloseCooking(); //Close Cooking Param
  delete[] Vertices; //Free Vertices
  delete[] Triangles; //Free Indices
} //EndGenerateTrianglesFromDXMeshProcedure

///////////////////////////////////////////////////////
// Load A .X File
///////////////////////////////////////////////////////
bool CGRPhysics::LoadXFile(char* MeshFilename,ID3DXMesh* &Mesh)
{
  //Zero Mesh And Create Buffer
  Mesh = 0;
  ID3DXBuffer* MeshBuffer = 0;

  //Load And Optimize The Mesh                     !!!
  if (FAILED(D3DXLoadMeshFromX(MeshFilename,D3DXMESH_32BIT|D3DXMESH_MANAGED,d_pDevice,&MeshBuffer,0,0,0,&Mesh)))
  return false;

  Mesh->OptimizeInplace(D3DXMESHOPT_ATTRSORT|D3DXMESHOPT_COMPACT|
  D3DXMESHOPT_VERTEXCACHE,(DWORD*)MeshBuffer->GetBufferPointer(),0,0,0);

  //Release And Zero The Buffer
  MeshBuffer->Release();
  
  return true;
} //EndLoadXFileFunction

//-----------------------------------------------------------------------------
// Name: CGRPhysics::TrianglesFromDXMesh()
// Desc: 
//-----------------------------------------------------------------------------
void CGRPhysics::TrianglesFromDXMesh(ID3DXMesh* m_pMesh)
{
  //LPD3DXMESH _Mesh;
  //LoadXFile("\Models\\Maps\\Terrain\\Terrain.x",_Mesh);

  //Generate Physics Object
  GenerateTrianglesFromDXMesh(m_pMesh,PhysXCube,NxVec3(0,0,0));
  //m_Mesh = m_pMesh;
} //EndTrianglesFromDXMeshProcedure

//-----------------------------------------------------------------------------
// Name: CGRPhysics::TrianglesFromDXMesh()
// Desc: 
//-----------------------------------------------------------------------------
void CGRPhysics::TrianglesFromDXMeshVerb(ID3DXMesh* m_pMesh,NxActor* Actor,NxVec3 Pos)
{
  //LPD3DXMESH _Mesh;
  //LoadXFile("\Models\\Maps\\Terrain\\Terrain.x",_Mesh);

  //Generate Physics Object
  GenerateTrianglesFromDXMesh(m_pMesh,Actor,Pos);
  //m_Mesh = m_pMesh;
} //EndTrianglesFromDXMeshProcedure

//-----------------------------------------------------------------------------
// Name: CGRPhysics::SetActorGroup()
// Desc: 
//-----------------------------------------------------------------------------
void CGRPhysics::SetActorGroup(NxActor* Actor,NxCollisionGroup Group)
{
  NxU32 nbShapes = Actor->getNbShapes();
  NxShape* const* Shapes = Actor->getShapes();

  while (nbShapes--)
  {
    Shapes[nbShapes]->setGroup(Group);
  }
} //EndSetActorGroupProcedure

//-----------------------------------------------------------------------------
// Name: CGRPhysics::Init()
// Desc: 
//-----------------------------------------------------------------------------
void CGRPhysics::Init(DWORD Param)
{
  //Let The Scene Know To Check For Collision Between
  if (Param == 1)
  {
    m_pScene->setActorPairFlags(*m_pPhysXPlayer,*PhysXCube,NX_NOTIFY_ON_TOUCH);
    SetActorGroup(PhysXCube,groupA); //For Ray Cast
	PhysXCube->raiseBodyFlag(NX_BF_KINEMATIC); //Static Object Parameter


    m_pScene->setActorPairFlags(*M_Barrel->GetActor(),*PhysXCube,NX_NOTIFY_ON_TOUCH);
    SetActorGroup(M_Barrel->GetActor(),groupA); //For Ray Cast
	//M_Barrel->GetActor()->raiseBodyFlag(NX_BF_KINEMATIC); //Static Object Parameter

   // SetActorPair(M_Barrel->GetActor(),d_pPhysX->GetPhysXWorld(),NX_NOTIFY_ON_TOUCH);
   // SetRaycastGroup(m_pActor);
   // SetBodyFlag(m_pActor,NX_BF_KINEMATIC);

  }
} //EndInitProcedure

//-----------------------------------------------------------------------------
// Name: CGRPhysics::SetActorPair()
// Desc: 
//-----------------------------------------------------------------------------
void CGRPhysics::SetActorPair(NxActor* &ActorA,NxActor* ActorB,DWORD Param)
{
  m_pScene->setActorPairFlags(*ActorA,*ActorB,Param); //For PhysX Report And Collision Between
} //EndSetActorPairProcedure

//-----------------------------------------------------------------------------
// Name: CGRPhysics::SetRaycastGroup()
// Desc: 
//-----------------------------------------------------------------------------
void CGRPhysics::SetRaycastGroup(NxActor* &Actor)
{
  SetActorGroup(Actor,groupA); //Set For Raycast Contact Group 
} //EndSetRaycastGroupProcedure

//-----------------------------------------------------------------------------
// Name: CGRPhysics::SetBodyFlag()
// Desc: 
//-----------------------------------------------------------------------------
void CGRPhysics::SetBodyFlag(NxActor* &Actor,NxBodyFlag Flag)
{
  Actor->raiseBodyFlag(Flag); //Static Object Parameter
} //EndSetBodyFlagProcedure

//-----------------------------------------------------------------------------
// Name: CGRPhysics::MakeRaycast()
// Desc: 
//-----------------------------------------------------------------------------
void CGRPhysics::MakeRaycast(NxActor* Actor,NxU32 GroupFlag,bool RaycastAll)
{
  if (!RaycastAll) //Raycast Just Closest Shape
  {
    //Get Ray Origin
    NxVec3 orig = Actor->getCMassGlobalPosition();

    //Set Ray Direction
    NxRay Ray;
    Ray.orig = orig;

    Ray.dir = NxVec3(gr_Camera->PosX,gr_Camera->PosY+0.25,gr_Camera->PosZ);
    Ray.dir.normalize();

    NxRaycastHit Hit;
    //NxReal dist;

    //Get The Closest Shape
    NxShape* ClosestShape = m_pScene->raycastClosestShape(Ray,NX_ALL_SHAPES,Hit,GroupFlag);
    
    //Make RayCast With Closest Shape Geometry
    if (ClosestShape)
	{
      const NxVec3& WorldImpact = Hit.worldImpact; //Get Raycast Point
	  const NxVec3& WorldNormal = Hit.worldNormal; //Get Polygon Normal

      //dist = Hit.distance;
  
      Vec1 = D3DXVECTOR3(1.0f,1.0f,1.0f); //NxVec3_To_DXVec3(pos[0]);	
	  Vec2 = D3DXVECTOR3(1.0f,1.0f,1.0f); //NxVec3_To_DXVec3(pos[1]);
	  Vec3 = D3DXVECTOR3(1.0f,1.0f,1.0f); //NxVec3_To_DXVec3(pos[2]);



      //Create Decal On The Wall 
      if (gr_Weapon->Be_Shoot == true) //Render Gun Shoot Fire
      { 
	  
		 //This Vitia Barrel
		 //if (Hit.shape->getGlobalPosition() == NxVec3(0.0f,0.001f,0.0f))
		 if (Hit.shape->isCapsule())
		 {
           //Create Barrel Sparks
		   M_Barrel->CreateSparks(NxVec3_To_DXVec3(WorldImpact));
		   M_Barrel->AddForce();
		 }
         else
		 {
           //Create Decal
           O_Decal[Num_Decals] = new CGRDecal(d_pDevice,1.0f,1.0f,false);
	       O_Decal[Num_Decals]->SetCoord(NxVec3_To_DXVec3(WorldImpact),NxVec3_To_DXVec3(WorldNormal),Vec1,Vec2);

	       Num_Decals++;
		 }
          
		 //Playback Richoshet From Sound Container	 
         gr_SoundsEff->PlaybackReset("Richoshet"); 
      }

	  Lbl_Raycast->Caption("PhysX Raycast: OK");
    } //End If (ClosestShape)
	else { Lbl_Raycast->Caption("PhysX Raycast: NULL"); }

  


    //Create Tube While Gun Fire
    if (gr_Weapon->Be_Shoot == true) //Be Gun Shoot Fire
    {
      //Create Gun Tube
      gr_Tube[Num_Tubes] = new CGRTube(d_pDevice,m_pScene,L"\Models\\Weapons\\Beretta\\Beretta_Shell.x");

	  //Ignore Contact Between Player Cube And Gun Tube
	  m_pScene->setActorPairFlags(*m_pPhysXPlayer,*gr_Tube[Num_Tubes]->GetActor(),NX_IGNORE_PAIR);
      //m_pScene->setActorPairFlags(*gr_Tube[Num_Tubes]->GetActor(),*gr_Tube[Num_Tubes]->GetActor(),NX_IGNORE_PAIR);

	  //Set Contact Respond Between Gun Tube And Level Geometry
	  m_pScene->setActorPairFlags(*PhysXCube,*gr_Tube[Num_Tubes]->GetActor(),NX_NOTIFY_ON_TOUCH);

	  Num_Tubes++;
	}


    //Draw Decals And Tube Array Here
    for (int I = 0; I < Num_Decals; ++I)
    {
      if (O_Decal[I] != NULL) 
	  { 
	    O_Decal[I]->Draw();
      }
	}







  }
  else //Raycast All Shapes Into The Scene
  {
    //Get Ray Origin
    NxVec3 orig = Actor->getCMassGlobalPosition();

    //Get Ray Direction
    NxVec3 dir;
    NxMat33 m = Actor->getGlobalOrientation();
    m.getColumn(0,dir);
    dir = -dir;
    NxRay Ray(orig,dir);

    //Get All Shapes
    NxU32 nbShapes = m_pScene->raycastAllShapes(Ray,gRaycastReport,NX_ALL_SHAPES,GroupFlag);
  }

} //EndMakeRaycastProcedure

//-----------------------------------------------------------------------------
// Name: CGRPhysics::RampBullet()
// Desc: 
//-----------------------------------------------------------------------------
void CGRPhysics::RampBullet()
{
 
 /* if (m_pBoxMesh != NULL) 
  {
    //Set The World Matrix
    D3DXMATRIX World;
    D3DXMatrixIdentity(&World);
    d_pDevice->SetTransform(D3DTS_WORLD,&World);

    D3DXMATRIX Origin;
    D3DXMatrixIdentity(&Origin);
    D3DXMatrixRotationX(&Origin, 1.57f);
    m_pPhysBox->getGlobalPose().getColumnMajor44(World);
  
    Origin*= World;
    d_pDevice->SetTransform(D3DTS_WORLD,&Origin);

	
    m_pBoxMesh->DrawSubset(0);
  }*/
 

  //Show Tube Contact Report Label
  Lbl_TubeReport->FontColor = clWhite;
  Lbl_TubeReport->Caption("Shell Contact Report: NULL");
  Lbl_TubeReport->Update();

  if (ObjectsColliding == true)
  {
    TCHAR m_pDebugStr[50]; //String Pointer

    //Prepare For Debug Output Normal X Parameters
    const int Fmt_Str = sizeof(m_pDebugStr)/sizeof(TCHAR);
	_sntprintf(m_pDebugStr,Fmt_Str,_T("PhysX Contact Report: OK"),0,0,0,0,0,0);
    m_pDebugStr[Fmt_Str-1] = TEXT('\0');

    Lbl_Report->Caption(m_pDebugStr); //Debug Output Normal X Position*/
  }
  else 
  {
    TCHAR m_pDebugStr[50]; //String Pointer

    //Prepare For Debug Output Normal X Parameters
    const int Fmt_Str = sizeof(m_pDebugStr)/sizeof(TCHAR);
	_sntprintf(m_pDebugStr,Fmt_Str,_T("PhysX Contact Report: FALSE"),0,0,0,0,0,0);
    m_pDebugStr[Fmt_Str-1] = TEXT('\0');

    Lbl_Report->Caption(m_pDebugStr); //Debug Output Normal X Position*/
  }



  //Debugging Player Position Here:
  TCHAR m_pDebugStr[50]; //String Pointer

  //Prepare For Debug Output Normal X Parameters
  const int Fmt_Str = sizeof(m_pDebugStr)/sizeof(TCHAR);
  _sntprintf(m_pDebugStr,Fmt_Str,_T("Player Position X: %f"),gr_Camera->Position.x,0,0,0,0,0);
  m_pDebugStr[Fmt_Str-1] = TEXT('\0');

  Lbl_PosX->Caption(m_pDebugStr); //Debug Output Normal X Position
 
  //Prepare For Debug Output Normal Y Parameters
  _sntprintf(m_pDebugStr,Fmt_Str,_T("Player Position Y: %f"),gr_Camera->Position.y,0,0,0,0,0);
  m_pDebugStr[Fmt_Str-1] = TEXT('\0');

  Lbl_PosY->Caption(m_pDebugStr); //Debug Output Normal Y Position

  //Prepare For Debug Output Normal Z Parameters
  _sntprintf(m_pDebugStr,Fmt_Str,_T("Player Position Z: %f"),gr_Camera->Position.z,0,0,0,0,0);
  m_pDebugStr[Fmt_Str-1] = TEXT('\0');

  Lbl_PosZ->Caption(m_pDebugStr); //Debug Output Normal Z Position


   if (M_Barrel != NULL) M_Barrel->Draw(); //Render Simple Map Model

    //Draw Tube Array Here
    for (int I = 0; I < Num_Tubes; ++I)
    {
      if (gr_Tube[I] != NULL) 
	  { 
	    gr_Tube[I]->Draw();
      }
	}


  MakeRaycast(m_pPhysXPlayer,groupFlagA|groupFlagB,false);
} //EndRampBulletProcedure

//-----------------------------------------------------------------------------
