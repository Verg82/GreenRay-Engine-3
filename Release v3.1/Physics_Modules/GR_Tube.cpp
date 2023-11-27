/*******************************************************************************
// It's Unofficial Version Of The GreenRay Engine v.3.0.0.2
// GR_Tube.cpp
// It's The GreenRay Shotted Tube Engine.
// Made Specially For Implementing Shotted Tube Animation With Physics.
// Date Creation: 29 August 2005
// Last Changing: 07 April 2009
// Licensed By: (C)DDD Digital Dreams Development And (C)3D Studios Inc., 2009
*******************************************************************************/
#include <GR_Tube.h>


////////////////////////////////////////////////////////////////////////////////
// External Definitions And Prototypes 
////////////////////////////////////////////////////////////////////////////////
//Vertex Declarations For Another X-Files Converted From 3DS Max
D3DVERTEXELEMENT9 m_pShellVertDecl[] =
{
  {0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
  {0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,   0},
  {0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
  //{0, 32, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT,  0},
  //{0, 44, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL, 0},	  
  D3DDECL_END()
};

//////////////////////////////////////////////////////////////////////////////
// Construction/Destruction For CGRTube Class
//////////////////////////////////////////////////////////////////////////////
CGRTube::CGRTube(LPDIRECT3DDEVICE9 pDevice,NxScene* pScene,LPCWSTR File_Name)
{
  d_pDevice = pDevice; //Apropritate Device Pointer To Class
  d_pScene = pScene; //Apropritate Scene Pointer To Class

  //Set Default Parameters
  m_pShell = NULL;
  m_pPhysXTube = NULL;
  m_pShellSmoke = NULL;

  Tube_Radius = 0.095f; //Set Tube Radius
  Tube_Height = 0.36f; //Set Tube Height
  Dropped = false;
  Rotation = 0.0f;

  //Create The Shell Cylinder Mesh
  { //Create Shell Model
    m_pShell = new CGRModel(); //Construct A Shell Model Class
    //if (m_pTexPath != NULL) 
	m_pShell->SetTexturePath("\Textures\\Beretta\\Default\\"); //Set Model Texture Path

    //Create Vertex Declaration
    d_pDevice->CreateVertexDeclaration(m_pShellVertDecl,&pVertDecl);
 
    //Load An Object
    if (FAILED(m_pShell->Create(d_pDevice,File_Name)))
    {
      ShowMessage("GR_Shell.cpp: Error Load Shell Model..");
    }

    //Set A Reasonable Vertex Type
    m_pShell->SetVertexDecl(m_pShellVertDecl);
    
    //Initialize The Vertex Buffers For The File-Based Objects
    m_pShell->RestoreObjects();
  } //End Create Shell

  //Create Shell Smoke
  CreateShellSmoke();


  //Create PhysX Object
  //Define Tube Physics Object
  NxBodyDesc CapsuleBodyDesc;
  CapsuleBodyDesc.angularDamping = 0.1f;
  CapsuleBodyDesc.linearVelocity = NxVec3(0.0f, 0.0f, 0.0f);
  
  NxCapsuleShapeDesc CapsuleDesc;
  CapsuleDesc.height = Tube_Height+0.3f;
  CapsuleDesc.radius = Tube_Radius+0.3f;
  //CapsuleDesc.group = 2;

  CapsuleDesc.skinWidth = 0.01f;

  NxActorDesc CapsuleActorDesc;
  CapsuleActorDesc.shapes.pushBack(&CapsuleDesc);
  CapsuleActorDesc.body = &CapsuleBodyDesc;
  CapsuleActorDesc.density = 30.0f;

  CapsuleActorDesc.name = "Loh";

  //NxVec3 CylPos = m_pPhysXPlayer->getGlobalPosition();
  //CapsuleActorDesc.globalPose.t = NxVec3(CylPos.x,CylPos.y,CylPos.z+14.0f);
  
  //Add New Tube Object To Physics Scene
  m_pPhysXTube = d_pScene->createActor(CapsuleActorDesc);
  //SetActorGroup(m_pPhysXTube,groupB);
  


  //Set Startup Matrices And Tube Position
  D3DXMATRIXA16 mTransform;
  D3DXMATRIXA16 mInvView,mWorld;

  //Inverse Matrix Here
  D3DXMatrixIdentity(&mInvView);
  D3DXMatrixIdentity(&mWorld);
  D3DXMatrixInverse(&mInvView,NULL,&gr_Camera->View);

  //Set Model Postition On Screen Space
  D3DXVECTOR4 Pos(0.47f, -0.15f, 2.5f, 1.0f);
  D3DXVec4Transform(&Pos,&Pos,&mInvView);
  
  mWorld._41 = Pos.x;
  mWorld._42 = Pos.y;
  mWorld._43 = Pos.z;
  //mWorld._44 = 1.5f;

  //D3DXMatrixTranslation(&mWorld,Pos.x,Pos.y,Pos.z);

  //Multiply Matrices
  mWorld*= mInvView*gr_Camera->View; 

  D3DXMatrixIdentity(&mTransform);
  D3DXMatrixInverse(&mTransform,NULL,&gr_Camera->View);

  mTransform._41 = mWorld._41; 
  mTransform._42 = mWorld._42;  
  mTransform._43 = mWorld._43;
 
  //Set Transform And Draw Model Here
  //d_pDevice->SetTransform(D3DTS_WORLD,&mTransform);
  //m_pTube->DrawSubset(0); //Draw Model

  //Set Matrices To PhysX
  NxMat34 Matr;
  Matr.setColumnMajor44(mTransform);
  m_pPhysXTube->setGlobalPose(Matr);

  //Add Force To Shell
  m_pPhysXTube->addForce(NxVec3(500.0f, 13000.0f, 0.0f));

  //Add Rotation Moment To Shell
  m_pPhysXTube->addTorque(NxVec3(1000.0f, 15000.0f, 65000.0f));

  Rotation = 1.57f;

  //Set All Members Of The World Matrix Back To Zero
  D3DXMatrixIdentity(&mInvView);
  d_pDevice->SetTransform(D3DTS_WORLD,&mInvView);
  D3DXMatrixIdentity(&mWorld);
  d_pDevice->SetTransform(D3DTS_WORLD,&mWorld);
  D3DXMatrixIdentity(&mTransform);
  d_pDevice->SetTransform(D3DTS_WORLD,&mTransform);


} //EndConstructionDirectives

CGRTube::~CGRTube()
{
} //EndDestructionDirectives

//-----------------------------------------------------------------------------
// Name: CGRTube::Release()
// Desc: 
//-----------------------------------------------------------------------------
void CGRTube::Release()
{

  if (m_pShell != NULL) { m_pShell = NULL; } //Release Tube Object
  d_pDevice = NULL; //NULL Device Pointer
} //EndReleaseProcedure

//-----------------------------------------------------------------------------
// Name: CGRTube::Draw()
// Desc: 
//-----------------------------------------------------------------------------
void CGRTube::Draw()
{
  D3DXMATRIX Offset;

  if (m_pShell != NULL) 
  {
	//Set The World Matrix
	//D3DXMatrixIdentity(&mPos);
	//d_pDevice->SetTransform(D3DTS_WORLD,&mPos);

	//Cylinders created with the D3DXCreateCylinder() object are created sideways,
	//so we must offset the rotation by 45 degrees to match the PhysX matrix

	//Retrieve capsule transformation, offset it, apply it, and render the cylinder
	D3DXMatrixIdentity(&Offset);
	//D3DXMatrixRotationX(&Offset,1.57f);
	D3DXMatrixRotationX(&Offset,Rotation);
	if (m_pPhysXTube != NULL) m_pPhysXTube->getGlobalPose().getColumnMajor44(mPos);

	Offset*= mPos;

	//Transform And Draw Shell Model Here:
	d_pDevice->SetTransform(D3DTS_WORLD,&Offset);

    d_pDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_CCW);
    d_pDevice->SetRenderState(D3DRS_LIGHTING,TRUE);

    //Rendering Model Here
    LPD3DXMESH m_pMesh = m_pShell->GetLocalMesh();
    
    for (DWORD I = 0; I < m_pShell->m_dwNumMaterials; ++I)
    {
      //Set Material And Texture For The Mesh Object
      //d_pDevice->SetMaterial(&m_pShell->m_pMaterials[I]);
	  d_pDevice->SetTexture(0,m_pShell->m_pTextures[I]);
      m_pMesh->DrawSubset(I);
    }
    //if (m_pShell != NULL) m_pShell->Render();
  }

  //Update Shell Smoke Particle
  if (m_pShellSmoke != NULL)
  {
    m_pShellSmoke->Update();
	m_pShellSmoke->SetPosition(D3DXVECTOR3(Offset._41+0.24,Offset._42+0.2f,Offset._43));
    
	if (m_pShellSmoke->Is_Updated) 
	{
      m_pShellSmoke->Release(); 
	  ReleaseClass(m_pShellSmoke); 
	  DeleteClass(m_pShellSmoke);
	  m_pShellSmoke = NULL;

	  //Release Shell PhysX Animation Actors By Time:
      if (Dropped == true)
	  {
	    ReleaseActor(); //Release Actor
	  }
	  else
	  {
	    //Release PhysX Actor And Shell Mesh
		ReleaseActor(); //Release Actor
        if (m_pShell != NULL) { DeleteClass(m_pShell); m_pShell = NULL; } //Release Shell Object
	  }
	}
  } //End Update Shell Smoke Particle

  //Draw Some Shell Smoke Particles
  if (m_pShellSmoke != NULL)
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
	// Render Particle System
	//

    d_pDevice->SetTexture(0,m_pShellSmoke->GetTexture());
	m_pShellSmoke->Render();

	//
    // Reset Render States...
	//
    //d_pDevice->SetRenderState(D3DRS_ZENABLE,TRUE);
    d_pDevice->SetRenderState(D3DRS_ZWRITEENABLE,TRUE);
    d_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,FALSE);
  } //EndDrawParticles
} //EndDrawProcedure

//-----------------------------------------------------------------------------
// Name: CGRTube::ApplyForceToActor()
// Desc: 
//-----------------------------------------------------------------------------
NxVec3 CGRTube::ApplyForceToActor(NxActor* Actor,const NxVec3 &ForceDir,const NxReal ForceStrength)
{
  NxVec3 ForceVec = ForceStrength*ForceDir*gr_Timer->GetTime();
  Actor->addForce(ForceVec);
  return ForceVec;
} //EndApplyForceToActorFunction

//-----------------------------------------------------------------------------
// Name: CGRTube::GetActor()
// Desc: 
//-----------------------------------------------------------------------------
NxActor* CGRTube::GetActor()
{
  return m_pPhysXTube;

  //m_pPhysXTube->getName();
} //EndGetActorFunction

//-----------------------------------------------------------------------------
// Name: CGRTube::GetActorName()
// Desc: 
//-----------------------------------------------------------------------------
TCHAR* CGRTube::GetActorName()
{
  return (char*)m_pPhysXTube->getName();
} //EndGetActorNameFunction

//-----------------------------------------------------------------------------
// Name: CGRTube::ReleaseActor()
// Desc: 
//-----------------------------------------------------------------------------
void CGRTube::ReleaseActor()
{
  if (m_pPhysXTube != NULL)
  {
    //Set Last Object Position
	m_pPhysXTube->getGlobalPose().getColumnMajor44(mPos);

	//Release PhysX Actor
	//m_pPhysXTube->releaseShape;
    m_pPhysXTube = NULL;
	  
	//Stop And Release Timer
	//m_pTimer->Stop();
	//DeleteClass(m_pTimer);
    //m_pTimer = NULL;
  } 
} //EndReleaseActorProcedure

//-----------------------------------------------------------------------------
// Name: CGRTube::CreateShellSmoke()
// Desc: 
//-----------------------------------------------------------------------------
void CGRTube::CreateShellSmoke()
{
  //Create Smoke Particle System
  m_pShellSmoke = new CGRParticle(d_pDevice);
  m_pShellSmoke->SetTexture("\Textures\\Decals\\Smoke_Src.bmp");
  m_pShellSmoke->SetMaxParticles(46);
  m_pShellSmoke->SetNumToRelease(1);
  m_pShellSmoke->SetReleaseInterval(0.09f);
  m_pShellSmoke->SetLifeCycle(4.5f);
  m_pShellSmoke->SetSize(0.8f);
  m_pShellSmoke->SetColor(D3DXCOLOR(0.03f,0.03f,0.03f,1.0f));
 
  m_pShellSmoke->SetVelocity(D3DXVECTOR3(0.0f, 1.3f, 0.0f));
  m_pShellSmoke->SetGravity(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
  m_pShellSmoke->SetWind(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
  m_pShellSmoke->SetVelocityVar(0.1f);
  m_pShellSmoke->Init(); 
} //EndCreateShellSmokeProcedure
