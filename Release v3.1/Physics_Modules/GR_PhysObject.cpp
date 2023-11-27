/*******************************************************************************
// It's Unofficial Version Of The GreenRay Engine v.3.0.0.2
// GR_PhysObject.cpp
// It's The GreenRay PhysX Object Engine.
// Date Creation: 29 August 2005
// Last Changing: 16 February 2008
// Licensed By: (C)DDD Digital Dreams Development And (C)3D Studios Inc., 2009
*******************************************************************************/
#include <GR_PhysObject.h>

////////////////////////////////////////////////////////////////////////////////
// External Definitions And Prototypes 
////////////////////////////////////////////////////////////////////////////////
//Vertex Declarations For Another X-Files Converted From 3DS Max
D3DVERTEXELEMENT9 ObjVertDecl[] =
{
  {0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
  {0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,   0},
  {0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
  //{0, 32, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT,  0},
  //{0, 44, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL, 0},	  
  D3DDECL_END()
};

//////////////////////////////////////////////////////////////////////////////
// Construction/Destruction For CGRPhysObject Class
//////////////////////////////////////////////////////////////////////////////
CGRPhysObject::CGRPhysObject(LPDIRECT3DDEVICE9 pDevice,NxScene* pScene)
{
  d_pDevice = pDevice; //Apropritate Device Pointer To Class
  d_pScene = pScene; //Apropritate Scene Pointer To Class

  //Set Default Parameters
  m_pModel = NULL;
  m_pVertDecl = NULL;
  m_pSpark = NULL;
} //EndConstructionDirectives

CGRPhysObject::~CGRPhysObject()
{
} //EndDestructionDirectives

//-----------------------------------------------------------------------------
// Name: CGRPhysObject::Release()
// Desc: 
//-----------------------------------------------------------------------------
void CGRPhysObject::Release()
{
  
} //EndReleaseProcedure


//-----------------------------------------------------------------------------
// Name: CGRPhysObject::Create()
// Desc: 
//-----------------------------------------------------------------------------
void CGRPhysObject::Create(LPCWSTR File_Name)
{
  //Create Model Here:
  m_pModel = new CGRModel(); //Construct Model Class
  if (m_pTexPath != NULL) m_pModel->SetTexturePath(m_pTexPath); //Set Model Texture Path

  //Create Vertex Declaration
  d_pDevice->CreateVertexDeclaration(ObjVertDecl,&m_pVertDecl);
 
  //Load An Object
  if (FAILED(m_pModel->Create(d_pDevice,File_Name)))
  {
    ShowMessage("GR_PhysObject.cpp: Error Load Model..");
  }

  //Set A Reasonable Vertex Type
  m_pModel->SetVertexDecl(ObjVertDecl);
    
  //Initialize The Vertex Buffers For The File-Based Objects
  m_pModel->RestoreObjects();

  float Barrel_Radius = 1.6f;
  float Barrel_Height = 2.3f;



  //Create The Tube Cylinder Mesh
  //D3DXCreateCylinder(d_pDevice,1.9f,1.9f,1.9f+(1.9f*2.0f),20,20,&m_pTube,0);
  //D3DXCreateCylinder(d_pDevice,Barrel_Radius,Barrel_Radius,Barrel_Height+(Barrel_Radius*2.0f),20,20,&m_pTube,0);

  

  //Create PhysX Object
  //Define Tube Physics Object
  NxBodyDesc CapsuleBodyDesc;
  CapsuleBodyDesc.angularDamping = 1.0f;
  CapsuleBodyDesc.linearVelocity = NxVec3(0.0f, 0.0f, 0.0f);
  
  NxCapsuleShapeDesc CapsuleDesc;
  CapsuleDesc.height = Barrel_Height;
  CapsuleDesc.radius = Barrel_Radius;
  //CapsuleDesc.group = 2;

  CapsuleDesc.skinWidth = 0.05f;


  NxActorDesc CapsuleActorDesc;
  CapsuleActorDesc.shapes.pushBack(&CapsuleDesc);
  CapsuleActorDesc.body = &CapsuleBodyDesc;
  CapsuleActorDesc.density = 60.0f;
  CapsuleActorDesc.globalPose.t = NxVec3(0.0f, 0.0f, 0.0f);
  
  CapsuleActorDesc.name = "Barrel";

  //NxVec3 CylPos = m_pPhysXPlayer->getGlobalPosition();
  //CapsuleActorDesc.globalPose.t = NxVec3(CylPos.x,CylPos.y,CylPos.z+14.0f);
  
  //Add New Tube Object To Physics Scene
  m_pActor = d_pScene->createActor(CapsuleActorDesc);
} //EndCreateProcedure

//-----------------------------------------------------------------------------
// Name: CGRPhysObject::SetTexturePath()
// Desc: This Procedure For Setting Path Where Texture There Found
//-----------------------------------------------------------------------------
void CGRPhysObject::SetTexturePath(LPCSTR Tex_Path)
{
  m_pTexPath = Tex_Path;
} //EndSetTexturePathProcedure

//-----------------------------------------------------------------------------
// Name: CGRPhysObject::Draw()
// Desc: 
//-----------------------------------------------------------------------------
void CGRPhysObject::Draw()
{
  //Set Matrices
  D3DXMATRIX World,Offset;
  //Set The World Matrix
  D3DXMatrixIdentity(&World);
  d_pDevice->SetTransform(D3DTS_WORLD,&World);

  //Retrieve Object Transformation, Offset It, Apply It, And Render The Object
  D3DXMatrixIdentity(&Offset);
  D3DXMatrixRotationX(&Offset,1.57f);

  if (m_pActor != NULL) m_pActor->getGlobalPose().getColumnMajor44(World);
  Offset*= World;

  //Transform And Draw Object Model Here:
  d_pDevice->SetTransform(D3DTS_WORLD,&Offset);


  //Draw Object Here:
  d_pDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_CCW);
  d_pDevice->SetRenderState(D3DRS_LIGHTING,TRUE);

  //D3DXMATRIX Transf;
  //D3DXMatrixIdentity(&Transf);
	
  //Transform And Draw Barrel Model Here:
  //d_pDevice->SetTransform(D3DTS_WORLD,&Transf);

  //Rendering Model Here
  LPD3DXMESH m_pMesh = m_pModel->GetLocalMesh();
    
  for (DWORD I = 0; I < m_pModel->m_dwNumMaterials; ++I)
  {
    //Set Material And Texture For The Mesh Object
    //d_pDevice->SetMaterial(&m_pModel->m_pMaterials[I]);
	d_pDevice->SetTexture(0,m_pModel->m_pTextures[I]);
    m_pMesh->DrawSubset(I);
  }

  //m_pTube->DrawSubset(0);

  //if (m_pModel != NULL) m_pModel->Render();


  //Update Spark Particle
  if (m_pSpark != NULL)
  {
    m_pSpark->Update();

    if (m_pSpark->Is_Updated) 
	{
      m_pSpark->Release(); 
	  ReleaseClass(m_pSpark); 
	  DeleteClass(m_pSpark);
	  m_pSpark = NULL;
	}
  } //End Update Spark

  //Draw Some Spark
  if (m_pSpark != NULL)
  {
    //Set Render Matrices
    D3DXMATRIX Mat_Part;
    D3DXMatrixIdentity(&Mat_Part);
    d_pDevice->SetTransform(D3DTS_WORLD,&Mat_Part);

    //d_pDevice->SetRenderState(D3DRS_ZENABLE,TRUE);
	d_pDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE);
    d_pDevice->SetRenderState(D3DRS_LIGHTING,FALSE);
    d_pDevice->SetRenderState(D3DRS_ZWRITEENABLE,FALSE);
    d_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);

 	d_pDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
    //d_pDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
    d_pDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ONE);
	
	//
	// Render Spark
	//

    d_pDevice->SetTexture(0,m_pSpark->GetTexture());
	m_pSpark->Render();

	//
    // Reset Render States...
	//
    //d_pDevice->SetRenderState(D3DRS_ZENABLE,TRUE);
    d_pDevice->SetRenderState(D3DRS_ZWRITEENABLE,TRUE);
    d_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,FALSE);
  } //EndDrawParticles

} //EndDrawProcedure

//-----------------------------------------------------------------------------
// Name: CGRPhysObject::GetActor()
// Desc: 
//-----------------------------------------------------------------------------
NxActor* CGRPhysObject::GetActor()
{
  return m_pActor;
} //EndGetActorFunction

//-----------------------------------------------------------------------------
// Name: CGRPhysObject::GetMesh()
// Desc: 
//-----------------------------------------------------------------------------
LPD3DXMESH CGRPhysObject::GetMesh()
{
  return m_pModel->GetLocalMesh();
} //EndGetMeshFunction

//-----------------------------------------------------------------------------
// Name: CGRPhysObject::AddForce()
// Desc: 
//-----------------------------------------------------------------------------
void CGRPhysObject::AddForce()
{
  //Add Force To Object
  m_pActor->addForce(NxVec3(1500000.0f, 1300000.0f, 0.0f));

  //Add Rotation Moment To Object
  m_pActor->addTorque(NxVec3(10000.0f, 1500000.0f, 65000.0f));
} //EndAddForceProcedure

//-----------------------------------------------------------------------------
// Name: CGRPhysObject::CreateSparks()
// Desc: 
//-----------------------------------------------------------------------------
void CGRPhysObject::CreateSparks(D3DXVECTOR3 Pos)
{
  m_pSpark = new CGRParticle(d_pDevice);
  m_pSpark->SetTexture("\Textures\\Decals\\Particle.bmp");
  m_pSpark->SetMaxParticles(8);
  m_pSpark->SetNumToRelease(8);
  m_pSpark->SetReleaseInterval(0.05f);
  m_pSpark->SetLifeCycle(0.2f);
  m_pSpark->SetSize(0.3f);
  m_pSpark->SetColor(D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
  
  m_pSpark->SetPosition(Pos); //Set Sparks Position

  m_pSpark->SetVelocity(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
  m_pSpark->SetGravity(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
  m_pSpark->SetWind(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
  m_pSpark->SetVelocityVar(18.0f);
  m_pSpark->Init();
} //EndCreateSparksProcedure