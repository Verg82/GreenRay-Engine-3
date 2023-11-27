/*******************************************************************************
// It's Unofficial Version Of The GreenRay Engine v.3.0.0.2
// GR_Decals.cpp 
// It's The GreenRay Physics Engine.
// Made Specially For Implementing Bullet Holes Technology.
// Date Creation: 29 August 2005
// Last Changing: 09 March 2009
// Licensed By: (C)DDD Digital Dreams Development And (C)3D Studios Inc., 2009
*******************************************************************************/
#include <GR_Decals.h>

//////////////////////////////////////////////////////////////////////////////
// Construction/Destruction For CGRDecal Class
//////////////////////////////////////////////////////////////////////////////
CGRDecal::CGRDecal(LPDIRECT3DDEVICE9 pDevice,float Height,float Width,bool NoSmoke)
{
  d_pDevice = pDevice; //Apropritate Device Pointer To Class
  //D3DXCreatePolygon(d_pDevice,10,4,&m_pDecal,0); 
  CreateDecalSurface(Width,Height);

  //Set Default Parameters
  m_pParticle = NULL;
  m_pSmoke = NULL;
  m_pSpark = NULL;

  dwColor = D3DCOLOR_XRGB(255,255,255);

  //Create Decal Texture
  if (FAILED(D3DXCreateTextureFromFileEx(d_pDevice,"\Textures\\Decals\\BHole1.dds", 0,0,0,0,
  D3DFMT_UNKNOWN,D3DPOOL_MANAGED,D3DX_DEFAULT,D3DX_DEFAULT,dwColor,NULL,NULL,&m_pTexture)))
  {
    ShowMessage("GR_Decals.cpp: Can't Create Decal Texture..");
  }

  if (!NoSmoke)
  {
    //Create Particle System
    m_pParticle = new CGRParticle(d_pDevice);
    /*m_pParticle->SetTexture("Rock01.bmp");
    m_pParticle->SetMaxParticles(30);
    m_pParticle->SetNumToRelease(28);
    m_pParticle->SetReleaseInterval(0.05f);
    m_pParticle->SetLifeCycle(5.0f);
    m_pParticle->SetSize(0.5f);
    m_pParticle->SetColor(D3DXCOLOR(1.0f,1.0f,1.0f,1.0f));
    //m_pParticle->SetPosition(S_Pos); 
    m_pParticle->SetVelocity(D3DXVECTOR3(0.0f, -55.0f, 0.0f));
    m_pParticle->SetGravity(D3DXVECTOR3(0.0f, -55.0f, 0.0f));
    m_pParticle->SetWind(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
    m_pParticle->SetVelocityVar(8.7f);
    m_pParticle->SetCollisionPlane(D3DXVECTOR3(0.0f,1.0f,0.0f),D3DXVECTOR3(0.0f,0.0f,0.0f),1.0f,CR_STICK);
    m_pParticle->Init();*/
    
	m_pParticle->SetTexture("\Textures\\Decals\\Rock01.bmp");
    m_pParticle->SetMaxParticles(14);
    m_pParticle->SetNumToRelease(14);
    m_pParticle->SetReleaseInterval(0.05f);
    m_pParticle->SetLifeCycle(3.2f);
    m_pParticle->SetSize(0.6f);
    m_pParticle->SetColor(D3DXCOLOR(1.0f,1.0f,1.0f,1.0f));

    m_pParticle->SetVelocity(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
    m_pParticle->SetGravity(D3DXVECTOR3( 0.0f, -90.0f, 0.0f));
    m_pParticle->SetWind(D3DXVECTOR3(0.0f,0.0f,0.0f));
    m_pParticle->SetVelocityVar(15.0f);
    m_pParticle->SetCollisionPlane(D3DXVECTOR3(0.0f,1.0f,0.0f),D3DXVECTOR3(0.0f,0.0f,0.0f),1.0f,CR_STICK);
    m_pParticle->Init();

    //Create Smoke Particle System
    m_pSmoke = new CGRParticle(d_pDevice);
    m_pSmoke->SetTexture("\Textures\\Decals\\Smoke_Src.bmp");
    m_pSmoke->SetMaxParticles(5);
    m_pSmoke->SetNumToRelease(1);
    m_pSmoke->SetReleaseInterval(0.019f);
    m_pSmoke->SetLifeCycle(0.5f);
    m_pSmoke->SetSize(3.1f);
    m_pSmoke->SetColor(D3DXCOLOR(0.25f,0.25f,0.25f,1.0f));
 
    m_pSmoke->SetVelocity(D3DXVECTOR3(0.0f, 30.0f, 0.0f));
    m_pSmoke->SetGravity(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
    m_pSmoke->SetWind(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
    m_pSmoke->SetVelocityVar(4.5f);
    m_pSmoke->Init();

    //Create Spark Particle
    m_pSpark = new CGRParticle(d_pDevice);
    m_pSpark->SetTexture("\Textures\\Decals\\Spark_Src01.bmp");
    m_pSpark->SetMaxParticles(5);
    m_pSpark->SetNumToRelease(1);
    m_pSpark->SetReleaseInterval(0.01f);
    m_pSpark->SetLifeCycle(0.5f);
    m_pSpark->SetSize(0.6f);
    m_pSpark->SetColor(D3DXCOLOR(1.5f,1.5f,1.5f,1.0f));

    m_pSpark->SetVelocity(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
    m_pSpark->SetGravity(D3DXVECTOR3(0.0f, -100.0f, 0.0f));
    m_pSpark->SetWind(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
    m_pSpark->SetVelocityVar(60.0f);
    m_pSpark->SetCollisionPlane(D3DXVECTOR3(0.0f,1.0f,0.0f),D3DXVECTOR3(0.0f,0.0f,0.0f),1.0f,CR_BOUNCE);
    m_pSpark->Init();
  }
} //EndConstructionDirectives

CGRDecal::~CGRDecal()
{
} //EndDestructionDirectives

//-----------------------------------------------------------------------------
// Name: CGRDecal::Release()
// Desc: 
//-----------------------------------------------------------------------------
void CGRDecal::Release()
{
  //ReleaseClass(m_pDecal); //Free Decal
  if (m_pTexture != NULL) ReleaseClass(m_pTexture); 
  ReleaseClass(m_pVertexBuffer);
  d_pDevice = NULL; //NULL Device Pointer
} //EndReleaseProcedure

//-----------------------------------------------------------------------------
// Name: CGRSkyBox::CreateDecalSurface()
// Desc: Initialize Our Vertexes Structure
//-----------------------------------------------------------------------------
HRESULT CGRDecal::CreateDecalSurface(float fWidth,float fHeight)
{
  HRESULT _Result = S_OK;
  const FLOAT fDepth = 0.0f; //Z

  //Vertexes Description
  DECAL_VERTEX Vertexes[] = 
  {
	{-fWidth,  fHeight, fDepth, 0.0f, 0.0f}, //A
	{ fWidth,  fHeight, fDepth, 1.0f, 0.0f}, //B
	{ fWidth, -fHeight, fDepth, 1.0f, 1.0f}, //C

	{ fWidth, -fHeight, fDepth, 1.0f, 1.0f}, //C
	{-fWidth, -fHeight, fDepth, 0.0f, 1.0f}, //D
	{-fWidth,  fHeight, fDepth, 0.0f, 0.0f}, //A
     //X          Y        Z    TexU  TexV
  }; //End Vertexes Description

  //Create Vertex Buffer
  if (FAILED(d_pDevice->CreateVertexBuffer(6*sizeof(DECAL_VERTEX),0, 
  D3DFVF_DECALVERTEX,D3DPOOL_DEFAULT,&m_pVertexBuffer,NULL))) _Result = E_FAIL;

  //Lock The Vertex Buffer
  VOID* pBV;
  if (FAILED(m_pVertexBuffer->Lock(0,sizeof(Vertexes),(void**)&pBV,0))) _Result = E_FAIL;
  memcpy(pBV,Vertexes,sizeof(Vertexes)); //Copy Vertex Data To Memory
  m_pVertexBuffer->Unlock(); //Unlock The Vertex Buffer
  
  return _Result;
} //EndCreateSkyBoxFunction

//-----------------------------------------------------------------------------
// Name: CGRDecal::SetCoord()
// Desc: 
//-----------------------------------------------------------------------------
void CGRDecal::SetCoord(D3DXVECTOR3 vPos,D3DXVECTOR3 vNormal,D3DXVECTOR3 vVec1,D3DXVECTOR3 vVec2)
{ 
  Pos = D3DXVECTOR3(vPos); //Get RayCast Point Position
  Normal = D3DXVECTOR3(vNormal); //Get Normals

  //Get Polygon Vertices Vectors
  Vec_1 = D3DXVECTOR3(vVec1); 
  Vec_2 = D3DXVECTOR3(vVec2);

  //Set Particle Position
  Par_Pos.x = Pos.x;
  Par_Pos.y = Pos.y+0.0f;
  Par_Pos.z = Pos.z;
  if (m_pParticle != NULL) m_pParticle->SetPosition(Par_Pos); 
  if (m_pSpark != NULL) m_pSpark->SetPosition(Par_Pos); 
  if (m_pSmoke != NULL) m_pSmoke->SetPosition(D3DXVECTOR3(Par_Pos.x,Par_Pos.y+0.8f,Par_Pos.z));

  /*Pos.x = Pos.x+0.007f;
  Pos.y = Pos.y+0.007f;
  Pos.z = Pos.z+0.007f;

  if (Normal.x >= 1.0f) Pos.x = Pos.x+0.01f; 
  if (Normal.x <= -1.0f) Pos.x = Pos.x-0.01f; 

  if (Normal.x == 0.0f) Pos.x = Pos.x+0.01f; 
  if (Normal.x < 0.0f) Pos.x = Pos.x-0.01f; 

  if (Normal.y >= 1.0f) Pos.y = Pos.y+0.01f;
  if (Normal.y <= -1.0f) Pos.y = Pos.y-0.01f; 

  if (Normal.y == 0.0f) Pos.y = Pos.y+0.01f; 
  if (Normal.y < 0.0f) Pos.y = Pos.y-0.01f; 

  if (Normal.z >= 1.0f) Pos.z = Pos.z+0.01f; 
  if (Normal.z <= -1.0f) Pos.z = Pos.z-0.01f; 

  if (Normal.z == 0.0f) Pos.z = Pos.z+0.01f; 
  if (Normal.z < 0.0f) Pos.z = Pos.z-0.01f;*/

  //if (Normal == D3DXVECTOR3(0.0f,0.0f,1.0f)) Pos.z = Pos.z+1.1f;

  //Set Needed Matrices
  //D3DXMATRIXA16 Mat_Trans,Mat_Pos;
  
  //D3DXMatrixIdentity(&Mat_Trans);
  //D3DXMatrixIdentity(&Mat_Pos);

  //D3DXMatrixTranslation(&Mat_Rot,WorldNormal.x,WorldNormal.y,WorldNormal.z);
  //D3DXMatrixRotationAxis(&Mat_Rot,0.0,0.0,0.0);
 
  //D3DXMatrixTranslation(&Mat_Rot,Vec1.x, Vec1.y, Vec1.z);
  //D3DXMatrixTranslation(&Mat_Pos,WorldImpact.x, WorldImpact.y, WorldImpact.z);
	
  //Build Polygon Rotation Matrices
  //Set Needed Matrices
  D3DXVec3Normalize(&Normal,&Normal);

  //if (Normal = D3DXVECTOR3(0,1,0)) { Normal = D3DXVECTOR3(1,0,0); }

  D3DXVec3Cross(&Vec_1,&Normal,&Vec_2);
  D3DXVec3Cross(&Vec_2,&Vec_1,&Normal);
  D3DXVec3Normalize(&Vec_2,&Vec_2);
  D3DXVec3Normalize(&Vec_1,&Vec_1);

  Mat_Trans._11 = Vec_1.x;
  Mat_Trans._12 = Vec_1.y;
  Mat_Trans._13 = Vec_1.z;

  Mat_Trans._21 = Vec_2.x;
  Mat_Trans._22 = Vec_2.y;
  Mat_Trans._23 = Vec_2.z; 

  Mat_Trans._31 = Normal.x;
  Mat_Trans._32 = Normal.y;
  Mat_Trans._33 = Normal.z;

  Mat_Trans._41 = Pos.x;
  Mat_Trans._42 = Pos.y;
  Mat_Trans._43 = Pos.z;

  Mat_Trans._14 = 0.0f;
  Mat_Trans._24 = 0.0f;
  Mat_Trans._34 = 0.0f;
  Mat_Trans._44 = 1.0f;
} //EndSetCoordProcedure

//-----------------------------------------------------------------------------
// Name: CGRDecal::Draw()
// Desc: 
//-----------------------------------------------------------------------------
void CGRDecal::Draw()
{
  //Update Particle System
  if (m_pParticle != NULL)
  {
    m_pParticle->Update();

    if (m_pParticle->Is_Updated) 
	{
      m_pParticle->Release(); 
	  ReleaseClass(m_pParticle); 
	  DeleteClass(m_pParticle);
      m_pParticle = NULL;
	}
  } //End Update Particle System

  //Draw Some Particles
  if (m_pParticle != NULL)
  {
    //Set Render Matrices
    D3DXMATRIX Mat_Part;
    D3DXMatrixIdentity(&Mat_Part);
    d_pDevice->SetTransform(D3DTS_WORLD,&Mat_Part);

    //d_pDevice->SetRenderState(D3DRS_ZENABLE,TRUE);
    d_pDevice->SetRenderState(D3DRS_ZWRITEENABLE,FALSE);
    d_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);

 	d_pDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
    d_pDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
    //d_pDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ONE);
	
	//
	// Render Particle System
	//

    d_pDevice->SetTexture(0,m_pParticle->GetTexture());
	m_pParticle->Render();

	//
    // Reset Render States...
	//
    //d_pDevice->SetRenderState(D3DRS_ZENABLE,TRUE);
    d_pDevice->SetRenderState(D3DRS_ZWRITEENABLE,TRUE);
    d_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,FALSE);
  } //EndDrawParticles




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




  //Update Smoke Particle
  if (m_pSmoke != NULL)
  {
    m_pSmoke->Update();

    if (m_pSmoke->Is_Updated) 
	{
      m_pSmoke->Release(); 
	  ReleaseClass(m_pSmoke); 
	  DeleteClass(m_pSmoke);
	  m_pSmoke = NULL;
	}
  } //End Update Smoke Particle

  //Draw Some Smoke Particles
  if (m_pSmoke != NULL)
  {
    //Set Render Matrices
    D3DXMATRIX Mat_Part;
    D3DXMatrixIdentity(&Mat_Part);
    d_pDevice->SetTransform(D3DTS_WORLD,&Mat_Part);

    //d_pDevice->SetRenderState(D3DRS_ZENABLE,TRUE);
    d_pDevice->SetRenderState(D3DRS_ZWRITEENABLE,FALSE);
    d_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);

 	d_pDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
    //d_pDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
    d_pDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ONE);
	
	//
	// Render Particle System
	//

    d_pDevice->SetTexture(0,m_pSmoke->GetTexture());
	m_pSmoke->Render();

	//
    // Reset Render States...
	//
    //d_pDevice->SetRenderState(D3DRS_ZENABLE,TRUE);
    d_pDevice->SetRenderState(D3DRS_ZWRITEENABLE,TRUE);
    d_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,FALSE);
  } //EndDrawParticles




  //Setting Rendering Parameters
  d_pDevice->SetRenderState(D3DRS_LIGHTING,FALSE);
  d_pDevice->SetRenderState(D3DRS_ZENABLE,TRUE);
  d_pDevice->SetRenderState(D3DRS_ZWRITEENABLE,FALSE);  
  d_pDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_CCW);

  //Enable Alpha Blending So We Can Use Transparent Textures
  d_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);

  //Set How The Texture Should Be Blended (Use Alpha)
  d_pDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
  d_pDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
  
  //Set Perspective View For The Model In Space
  float Aspect_Ratio = 1.1f;
  D3DXMATRIXA16 Mat_Proj;
  D3DXMatrixPerspectiveFovLH(&Mat_Proj,/*D3DX_PI/4*/D3DXToRadian(60.0f),Aspect_Ratio,1.1f,10000.0f);
  d_pDevice->SetTransform(D3DTS_PROJECTION,&Mat_Proj);

  //Mat_Trans = (Mat_Rot*Mat_Pos);
  d_pDevice->SetTransform(D3DTS_WORLD,&Mat_Trans);

  //Set Vertex And Index Buffers To Render Decal
  d_pDevice->SetTexture(0,NULL);
  d_pDevice->SetStreamSource(0,m_pVertexBuffer,0,sizeof(DECAL_VERTEX));
  d_pDevice->SetFVF(D3DFVF_DECALVERTEX);

  //Set Texture And Draw Decal Mesh
  d_pDevice->SetTexture(0,m_pTexture);
  //d_pDevice->SetTextureStageState(0,D3DTSS_ALPHAOP,D3DTOP_MODULATE);
  d_pDevice->DrawPrimitive(D3DPT_TRIANGLELIST,0,2);
  //m_pDecal->DrawSubset(0); //Draw Decal
} //EndDrawProcedure