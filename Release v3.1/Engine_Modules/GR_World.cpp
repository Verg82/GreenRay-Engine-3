/*******************************************************************************
// It's Unofficial Version Of The GreenRay Engine v.3.0.0.3
// GR_World.cpp
// It's The GreenRay WorldMap Geometry Transform Engine Source File.
// Made Specially For Implementing Only WorldMap Geometry Effects.
// Programming By: Verg
// Date Creation: 29 August 2005
// Last Changing: 26 August 2007
// Author Rights By: Zie Technology And Lucky's Flash Studio Inc.,
// Licensed By: (C)DDD Digital Dreams Development And (C)3D Studios Inc., 2007
*******************************************************************************/
#include <GR_World.h>

////////////////////////////////////////////////////////////////////////////////
// External Definitions And Prototypes 
////////////////////////////////////////////////////////////////////////////////
//Vertex Declarations For Another X-Files Converted From 3DS Max
D3DVERTEXELEMENT9 g_pVertDecl[] =
{
  {0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
  {0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,   0},
  {0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
  //{0, 32, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT,  0},
  //{0, 44, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL, 0},	  
  D3DDECL_END()
};

//////////////////////////////////////////////////////////////////////////////
// Construction/Destruction For CGRSkyBox Class
//////////////////////////////////////////////////////////////////////////////
CGRSkyBox::CGRSkyBox(LPDIRECT3DDEVICE9 pDevice)
{
  d_pDevice = pDevice; //Apropritate Device Pointer To Class
  m_pVertexBuffer = NULL; //Prepare Vertex Buffer
  m_pIndexBuffer = NULL; //Prepare Index Buffer
  memset(m_pTex,0x00,sizeof(m_pTex));
} //EndConstructionDirectives

CGRSkyBox::~CGRSkyBox()
{  
} //EndDestructionDirectives

//------------------------------------------------------------------------------
// Name: CGRSkyBox::Release()
// Desc: 
//------------------------------------------------------------------------------
void CGRSkyBox::Release()
{
  for (UINT I = 0; I < 6; I++) { ReleaseClass(m_pTex[I]); m_TexPath[I] = NULL; } //Free Textures
  ReleaseClass(m_pVertexBuffer); //Free Vertex Buffer
  ReleaseClass(m_pIndexBuffer); //Free Index Buffer
  d_pDevice = NULL; //NULL Device Pointer
} //EndReleaseProcedure

//-----------------------------------------------------------------------------
// Name: CGRSkyBox::SetTextures()
// Desc: Loading And Init Textures
//-----------------------------------------------------------------------------
void CGRSkyBox::SetTextures(TCHAR* _TexBack,TCHAR* _TexLeft,
TCHAR* _TexFront,TCHAR* _TexRight,TCHAR* _TexFloor,TCHAR* _TexCeiling)
{
  m_TexPath[0] = _TexBack;
  m_TexPath[1] = _TexLeft;
  m_TexPath[2] = _TexFront;
  m_TexPath[3] = _TexRight;
  m_TexPath[4] = _TexFloor;
  m_TexPath[5] = _TexCeiling;

  //Create Some Textures
  for (UINT I = 0; I < 6; I++)
  {
    if (FAILED(D3DXCreateTextureFromFileEx(d_pDevice,m_TexPath[I],D3DX_DEFAULT,D3DX_DEFAULT,
    1,0,D3DFMT_UNKNOWN,D3DPOOL_MANAGED,D3DX_DEFAULT,D3DX_DEFAULT,0,NULL,NULL,&m_pTex[I])))
	{
	  ShowMessage("GRWorld.cpp: Can't Load SkyBox Texture");
	}
  }
} //EndSetTexturesProcedure

//-----------------------------------------------------------------------------
// Name: CGRSkyBox::CreateBox()
// Desc: Initialize Our Vertexes Structure
//-----------------------------------------------------------------------------
HRESULT CGRSkyBox::CreateBox(float fWidth,float fHeight,float fDepth)
{
  m_pWidth = fWidth;
  m_pHeight = fHeight;
  m_pDepth = fDepth;
  SKYBOX_VERTEX Vertexes[] =
  {
    //Width   Height   Depth  tu(W) tv(H)
    //Back Wall
	{ fWidth, 0.0f,    0.0f, 0.0f, 1.0f, }, //A
    { fWidth, fHeight, 0.0f, 0.0f, 0.0f, }, //B
    { 0.0f,   fHeight, 0.0f, 1.0f, 0.0f, }, //C
	{ 0.0f,   0.0f,    0.0f, 1.0f, 1.0f, }, //D
   
	//Left Wall
    { 0.0f, 0.0f,    0.0f,   0.0f, 1.0f, }, //A2
    { 0.0f, fHeight, 0.0f,   0.0f, 0.0f, }, //B2
    { 0.0f, fHeight, fDepth, 1.0f, 0.0f, }, //C2
	{ 0.0f, 0.0f,    fDepth, 1.0f, 1.0f, }, //D2
    
	//Front Wall
	{ 0.0f,   0.0f,    fDepth, 0.0f, 1.0f, }, //A3
    { 0.0f,   fHeight, fDepth, 0.0f, 0.0f, }, //B3
    { fWidth, fHeight, fDepth, 1.0f, 0.0f, }, //C3
	{ fWidth, 0.0f,    fDepth, 1.0f, 1.0f, }, //D3
    
	//Right Wall
    { fWidth, 0.0f,    fDepth, 0.0f, 1.0f, }, //A4  
    { fWidth, fHeight, fDepth, 0.0f, 0.0f, }, //B4
    { fWidth, fHeight, 0.0f,   1.0f, 0.0f, }, //C4
	{ fWidth, 0.0f,    0.0f,   1.0f, 1.0f, }, //D4
    
	//Floor    
    { fWidth, 0.0f, 0.0f,   1.0f, 1.0f, }, //A5
    { 0.0f,   0.0f, 0.0f,   0.0f, 1.0f, }, //B5
    { 0.0f,   0.0f, fDepth, 0.0f, 0.0f, }, //C5
	{ fWidth, 0.0f, fDepth, 1.0f, 0.0f, }, //D5
     
	//Ceiling
    { fWidth, fHeight, fDepth, 1.0f, 1.0f, }, //A6 
    { 0.0f,   fHeight, fDepth, 0.0f, 1.0f, }, //B6
    { 0.0f,   fHeight, 0.0f,   0.0f, 0.0f, }, //C6
	{ fWidth, fHeight, 0.0f,   1.0f, 0.0f, }, //D6
    //  x        y       z     tu(W) tv(H)
  }; //End Vertexes Description

  const unsigned short Index[] = 
  {
  0,1,2,    2,3,0,
  4,5,6,    6,7,4,
  8,9,10,   10,11,8,
  12,13,14, 14,15,12,
  16,17,18, 18,19,16,
  20,21,22, 22,23,20, 
  }; //End Indexes Description
	
  //For Loading Textures Previous Declarations

  //Create Vertex Buffer
  if (FAILED(d_pDevice->CreateVertexBuffer(36*sizeof(SKYBOX_VERTEX),0, 
  D3DFVF_SKYBOXVERTEX,D3DPOOL_DEFAULT,&m_pVertexBuffer,NULL))) return E_FAIL;

  //Lock The Vertex Buffer
  VOID* pBV;
  if (FAILED(m_pVertexBuffer->Lock(0,sizeof(Vertexes),(void**)&pBV,0))) return E_FAIL;
  
  memcpy(pBV,Vertexes,sizeof(Vertexes)); //Copy Vertex Data To Memory

  m_pVertexBuffer->Unlock(); //Unlock The Vertex Buffer
    
  //Create Index Buffer
  d_pDevice->CreateIndexBuffer(36*sizeof(Index),0,D3DFMT_INDEX16,D3DPOOL_DEFAULT,&m_pIndexBuffer,NULL);
    
  //Lock The Index Buffer
  VOID* pBI;
  m_pIndexBuffer->Lock(0,sizeof(Index),(void**)&pBI,0); 
    
  memcpy(pBI,Index,sizeof(Index)); //Copy Index Data To Memory
      
  m_pIndexBuffer->Unlock(); //Unlock The Index Buffer
 
  return S_OK;
} //EndCreateSkyBoxFunction

//-----------------------------------------------------------------------------
// Name: CGRSkyBox::RenderBox()
// Desc: Rendering The SkyBox Geometry With Textures
//-----------------------------------------------------------------------------
void CGRSkyBox::Render()
{  
  //Setting Rendering Parameters And Draw A SkyBox
  d_pDevice->SetRenderState(D3DRS_ZENABLE,FALSE);
  d_pDevice->SetRenderState(D3DRS_ZWRITEENABLE,FALSE);
  d_pDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_CCW);
  d_pDevice->SetRenderState(D3DRS_LIGHTING,FALSE);

  //VU Addr
  //d_pDevice->SetSamplerState(0,D3DSAMP_ADDRESSU,D3DTADDRESS_WRAP);
  //d_pDevice->SetSamplerState(0,D3DSAMP_ADDRESSV,D3DTADDRESS_WRAP);
  d_pDevice->SetSamplerState(0,D3DSAMP_ADDRESSU,D3DTADDRESS_CLAMP);
  d_pDevice->SetSamplerState(0,D3DSAMP_ADDRESSV,D3DTADDRESS_CLAMP);

  //Set Texture Mode
  //d_pDevice->SetTextureStageState(0,D3DTSS_COLORARG1,D3DTA_TEXTURE);
  //d_pDevice->SetTextureStageState(0,D3DTSS_COLOROP,D3DTOP_MODULATE);	
  //d_pDevice->SetTextureStageState(0,D3DTSS_COLOROP,D3DTOP_SELECTARG1);
  d_pDevice->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR);
  d_pDevice->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_LINEAR);
  //d_pDevice->SetSamplerState(0,D3DSAMP_MIPFILTER,D3DTEXF_POINT);

  //Set Perspective View For Spherical Show SkyBox In Space (60 Grad) 
  float Aspect_Ratio = 1.1f;
  D3DXMATRIXA16 _MatProj;
  //FLOAT fAspectRatio = (float)m_d3dsdBackBuffer.Width/(float)m_d3dsdBackBuffer.Height;
  D3DXMatrixPerspectiveFovLH(&_MatProj,/*D3DX_PI/4*/D3DXToRadian(60.0f),Aspect_Ratio,1.0f,1000.0f);
  d_pDevice->SetTransform(D3DTS_PROJECTION,&_MatProj); 

  //Translation Matrices For The SkyBox Object
  D3DXMATRIXA16 _MatTrans,_MatRotate,_MatPos,_MatCentered; 
  //                                        W           H           D
  D3DXMatrixTranslation(&_MatCentered,-m_pWidth/2,-m_pHeight/2,-m_pDepth/2); 
  D3DXMatrixRotationYawPitchRoll(&_MatRotate,D3DX_PI,0.0f,0.0f);
  D3DXMatrixTranslation(&_MatPos,gr_Camera->Position.x,gr_Camera->Position.y,gr_Camera->Position.z);
  _MatTrans = (_MatCentered*_MatRotate)*_MatPos;
  d_pDevice->SetTransform(D3DTS_WORLD,&_MatTrans); //Transform Object Position In Space

  //Draw The SkyBox Here
  //Set Vertex And Index Buffers To Render
  d_pDevice->SetTexture(0,NULL);
 
  d_pDevice->SetStreamSource(0,m_pVertexBuffer,0,sizeof(SKYBOX_VERTEX));
  d_pDevice->SetFVF(D3DFVF_SKYBOXVERTEX);
  d_pDevice->SetIndices(m_pIndexBuffer);
  
  //Output For Created Objects
  //Render BackWall
  if (m_pTex[0] != NULL) d_pDevice->SetTexture(0,m_pTex[0]);
  d_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2 );
  
  //Render LeftWall
  if (m_pTex[1] != NULL) d_pDevice->SetTexture(0,m_pTex[1]);
  d_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 4, 0, 4, 0, 2 ); //Offset To 4 Base Vertex Index 

  //Render FrontWall
  if (m_pTex[2] != NULL) d_pDevice->SetTexture(0,m_pTex[2]);
  d_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 8, 0, 4, 0, 2 ); //Offset To 8 Base Vertex Index 

  //Render RightWall
  if (m_pTex[3] != NULL) d_pDevice->SetTexture(0,m_pTex[3]);
  d_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 12, 0, 4, 0, 2 ); //Offset To 12 Base Vertex Index 

  //Render Floor
  if (m_pTex[4] != NULL) d_pDevice->SetTexture(0,m_pTex[4]);
  d_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 16, 0, 4, 0, 2 ); //Offset To 16 Base Vertex Index

  //Render Ceiling
  if (m_pTex[5] != NULL) d_pDevice->SetTexture(0,m_pTex[5]);
  d_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 20, 0, 4, 0, 2 ); //Offset To 20 Base Vertex Index
} //EndRenderProcedure

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction For CGRMap Class
///////////////////////////////////////////////////////////////////////////////
CGRMap::CGRMap()
{
  m_pMap = NULL;
  pVertDecl = NULL;
  g_pEffect = NULL;
  g_pMaterials = NULL;
} //EndConstructionDirectives

CGRMap::~CGRMap()
{ 
} //EndDestructionDirectives

//-----------------------------------------------------------------------------
// Name: CGRMap::Create()
// Desc: 
//-----------------------------------------------------------------------------
void CGRMap::Create(LPDIRECT3DDEVICE9 pDevice,LPCWSTR File_Name)
{
  d_pDevice = pDevice; //Apropritate Device Pointer To Class
 
  //Create Map Location To Represent
  {
    m_pMap = new CGRModel(); //Construct A Map Model Class
    if (m_pTexPath != NULL) m_pMap->SetTexturePath(m_pTexPath); //Set Model Texture Path

    //Create Vertex Declaration
    d_pDevice->CreateVertexDeclaration(g_pVertDecl,&pVertDecl);
 
    //Load An Object To Cast
    if (FAILED(m_pMap->Create(d_pDevice,File_Name)))
    {
      ShowMessage("GR_World.cpp: Error Load Map Location Model..");
    }

    //Set A Reasonable Vertex Type
    m_pMap->SetVertexDecl(g_pVertDecl);
    //m_pMap->SetFVF(d_pDevice,MODEL_VERTEX_FVF);  

    //Initialize The Vertex Buffers For The File-Based Objects
    m_pMap->RestoreObjects();
  } //End Create Map-Model

  //Create Light Shader
  if (FAILED(D3DXCreateEffectFromFile(d_pDevice,TEXT("\Shaders\\PointLight.fx"), 
  NULL,NULL,D3DXFX_NOT_CLONEABLE,NULL,&g_pEffect,NULL)))
  {
    ShowMessage("GR_World.cpp: Error Load Shader File..");
  }

  //Load The Texture Into LightTexture
  D3DXCreateTextureFromFile(d_pDevice,"\Textures\\LightMaps\\FlashLight.bmp",&LightTexture);

} //EndCreateProcedure

//-----------------------------------------------------------------------------
// Name: CGRMap::Release()
// Desc: 
//-----------------------------------------------------------------------------
void CGRMap::Release()
{
  //Free Map Model
  if (m_pMap != NULL) m_pMap = NULL;
} //EndReleaseProcedure

//-----------------------------------------------------------------------------
// Name: CGRMap::SetTexturePath()
// Desc: This Procedure For Setting Path Where Texture There Found
//-----------------------------------------------------------------------------
void CGRMap::SetTexturePath(LPCSTR Tex_Path)
{
  m_pTexPath = Tex_Path;
} //EndSetTexturePathProcedure

//-----------------------------------------------------------------------------
// Name: CGRMap::Render()
// Desc: 
//-----------------------------------------------------------------------------
void CGRMap::Render()
{
  //Render State For Until Rendering Mesh Object
  d_pDevice->SetRenderState(D3DRS_ZENABLE,TRUE);
  d_pDevice->SetRenderState(D3DRS_ZWRITEENABLE,TRUE);
  d_pDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_CCW);
  d_pDevice->SetRenderState(D3DRS_LIGHTING,FALSE);

  //VU Addr
  d_pDevice->SetSamplerState(0,D3DSAMP_ADDRESSU,D3DTADDRESS_WRAP);
  d_pDevice->SetSamplerState(0,D3DSAMP_ADDRESSV,D3DTADDRESS_WRAP);

  //Set Texturing State Parameters
  //d_pDevice->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR);
  //d_pDevice->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_LINEAR);
  
  //Anisotropic Texture Filtering
  d_pDevice->SetSamplerState(0,D3DSAMP_MAXANISOTROPY,8);
  d_pDevice->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_ANISOTROPIC);
  d_pDevice->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_ANISOTROPIC);
  d_pDevice->SetSamplerState(0,D3DSAMP_MIPFILTER,D3DTEXF_ANISOTROPIC);

  //Set Perspective View For The Model In Space
  float Aspect_Ratio = 1.1f;
  D3DXMATRIXA16 Mat_Proj;
  D3DXMatrixPerspectiveFovLH(&Mat_Proj,/*D3DX_PI/4*/D3DXToRadian(60.0f),Aspect_Ratio,1.0f,100000.0f);
  d_pDevice->SetTransform(D3DTS_PROJECTION,&Mat_Proj);

  //Translation Matrices For The Model Object
  D3DXMATRIXA16 Mat_Pos;
  D3DXMatrixTranslation(&Mat_Pos,0.0f, 0.0f, 0.0f);
  d_pDevice->SetTransform(D3DTS_WORLD,&Mat_Pos); //Transform Object Position In Space

  //Set The World Matrix
  D3DXMATRIX World;
  D3DXMatrixIdentity(&World);

  //Set Dynamic Shader Variables
  g_pEffect->SetTechnique("Ambient");
  g_pEffect->SetMatrix("WorldViewProjection",&(World*gr_Camera->View*Mat_Proj));
  g_pEffect->SetMatrix("WorldView",&(World*Mat_Pos));
  g_pEffect->SetMatrix("Proj",&(Mat_Proj));
  g_pEffect->SetVector("MatColor",&(D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f)));
  g_pEffect->SetFloat("FarClip",60.0f);


  //Set The Texture Adjustment Matrix Offsets
  float XOffset = 0.5f+(0.5f/(float)256); //Width Of The Light Texture
  float YOffset = 0.5f+(0.5f/(float)256); //Height Of The Light Texture

  //Create The Texture Adjustment Matrix
  D3DXMATRIX TexMatrix;
  ZeroMemory(&TexMatrix,sizeof(D3DXMATRIX));
  TexMatrix._11 = 0.5f;
  TexMatrix._22 = -0.5f;
  TexMatrix._33 = 0.5f;
  TexMatrix._41 = XOffset;
  TexMatrix._42 = YOffset;
  TexMatrix._43 = 0.5f;
  TexMatrix._44 = 1.0f;	

  //Create The Projection Matrix For The Light
  D3DXMATRIX LightProj;
  D3DXMatrixPerspectiveFovLH(&LightProj,D3DX_PI/4,1.0f,0.0f,1000.0f);


  g_pEffect->SetMatrix("World",&(World));
  g_pEffect->SetMatrix("TexTransform",&(World*gr_Camera->View*LightProj*TexMatrix));
  g_pEffect->SetValue("EyePos",&(gr_Camera->Position),sizeof(D3DXVECTOR3));
  g_pEffect->SetTexture("LightTex",LightTexture);
  g_pEffect->SetFloat("LightFalloff",75.0f);
  g_pEffect->CommitChanges();


  //Rendering Model Here
  LPD3DXMESH m_pMesh = m_pMap->GetLocalMesh();
   
  //In The First Pass, We Want To Render The Scene With Simple Ambient Lighting
  UINT Pass,Passes;
  g_pEffect->Begin(&Passes,0);
  for (Pass = 0; Pass < Passes; Pass++)
  {
	g_pEffect->BeginPass(Pass);

	//Set And Draw Each Of The Materials In The Mesh
    for (DWORD I = 0; I < m_pMap->m_dwNumMaterials; ++I)
    {   
      //Set Texture For The Mesh Object
      if (m_pMap->m_pTextures[I]) g_pEffect->SetTexture("Texture",m_pMap->m_pTextures[I]);
	
      g_pEffect->CommitChanges();

      m_pMesh->DrawSubset(I);
    }
    g_pEffect->EndPass();
  }
  g_pEffect->End();

  //Set Light Positions
  D3DXVECTOR4 LightPositions[3];
  LightPositions[0] = D3DXVECTOR4(0.0f, 65.0f, 0.0f, 0.0f);
  LightPositions[1] = D3DXVECTOR4(-400.0f, 65.0f, 0.0f, 0.0f);
  LightPositions[2] = D3DXVECTOR4(gr_Camera->Position.x, gr_Camera->Position.y, gr_Camera->Position.z, 0.0f);

  //Set Light Color
  D3DXVECTOR4 LightColor[3];  //R      G      B     A
  LightColor[0] = D3DXVECTOR4(20.0f, 20.0f, 20.0f, 0.0f);
  LightColor[1] = D3DXVECTOR4(20.0f, 20.0f, 20.0f, 0.0f);
  LightColor[2] = D3DXVECTOR4(1.8f, 1.8f, 0.15f, 0.0f);

  //Set Light Ambient
  D3DXVECTOR4 LightAmbient[3];  //R     G      B     A
  LightAmbient[0] = D3DXVECTOR4(0.1f, 0.1f, 0.1f, 0.0f);
  LightAmbient[1] = D3DXVECTOR4(0.1f, 0.1f, 0.1f, 0.0f);
  LightAmbient[2] = D3DXVECTOR4(0.14f, 0.14f, 0.025f, 0.0f);



  //Set The Number Of Lights
  int Num_Lights = 2;

  //Set Num Of Light For Implement Shot Flare
  if (gr_Weapon->Be_Shoot == true) 
  {
    Num_Lights = 3;
  }
  else
  {
    Num_Lights = 2;
  } //End Shot Flare



  //Render The Scene With A Point Light
  for (int LightNum = 0; LightNum < Num_Lights; LightNum++)
  {
    g_pEffect->SetVector("LightColor",&(LightColor[LightNum]));
    g_pEffect->SetVector("AmbientA",&(LightAmbient[LightNum]));
   
	//Transform The Light Position Into View Space And Send It To The Shader
	D3DXVec4Transform(&LightPositions[LightNum],&LightPositions[LightNum],&Mat_Pos);
	g_pEffect->SetVector("LightView",&(LightPositions[LightNum]));

	//Set The Point Light Technique
	g_pEffect->SetTechnique("PointLight");
	g_pEffect->CommitChanges();

	//Finally, Render The Scene With A Point Light
	UINT iPass,iPasses;
	g_pEffect->Begin(&iPasses,0);
    
	for (iPass = 0; iPass < iPasses; ++iPass)
	{
      g_pEffect->BeginPass(iPass);

	  //Set And Draw Each Of The Materials In The Mesh
      for (DWORD I = 0; I < m_pMap->m_dwNumMaterials; ++I)
      {
        //Set Texture For The Mesh Object
		if (m_pMap->m_pTextures[I]) g_pEffect->SetTexture("Texture",m_pMap->m_pTextures[I]);
        g_pEffect->CommitChanges();

        m_pMesh->DrawSubset(I);
      }
      g_pEffect->EndPass();
	}
	g_pEffect->End();
  }



 /* //Set dynamic shader variables
  //ProjTextureEffect->SetTechnique("ProjectiveTexture");
  //ProjTextureEffect->SetMatrix("WorldViewProj",&(World*gr_Camera->View*Mat_Proj));
  ProjTextureEffect->SetMatrix("World",&(World));
  ProjTextureEffect->SetMatrix("TexTransform",&(World*gr_Camera->View*LightProj*TexMatrix));
  ProjTextureEffect->SetValue("EyePos", &(gr_Camera->Position),sizeof(D3DXVECTOR3));
  //ProjTextureEffect->SetTexture("ColorTex",RoomTexture);
  ProjTextureEffect->SetTexture("LightTex",LightTexture);
  ProjTextureEffect->SetFloat("LightFalloff",55.0f);
  ProjTextureEffect->CommitChanges();
  */



  //Begin The Shader Pass
 /* UINT uPass,uPasses;
  ProjTextureEffect->Begin(&uPasses,0);	
  for (uPass = 0; uPass < uPasses; uPass++)
  {
    ProjTextureEffect->BeginPass(uPass);

	  //Set And Draw Each Of The Materials In The Mesh
      for (DWORD I = 0; I < m_pMap->m_dwNumMaterials; ++I)
      {
        //Set Texture For The Mesh Object
        if (m_pMap->m_pTextures[I]) ProjTextureEffect->SetTexture("ColorTex",m_pMap->m_pTextures[I]);
        ProjTextureEffect->CommitChanges();

        m_pMesh->DrawSubset(I);   	
      }


		ProjTextureEffect->EndPass();
	}
	ProjTextureEffect->End();*/

} //EndRenderProcedure

//-----------------------------------------------------------------------------
// Name: CGRMap::GetLocalMeshGeometry()
// Desc: 
//-----------------------------------------------------------------------------
LPD3DXMESH CGRMap::GetLocalMeshGeometry()
{
  return m_pMap->GetLocalMesh();
} //EndGetLocalMeshGeometryFunction

//-----------------------------------------------------------------------------
// Name: CGRMap::GetLocalMeshGeometry()
// Desc: 
//-----------------------------------------------------------------------------
CGRModel* CGRMap::GetCGRModel()
{
  return m_pMap;
} //EndGetLocalMeshGeometryFunction

//-----------------------------------------------------------------------------