/*******************************************************************************
// It's Unofficial Version Of The GreenRay Engine v.3.0.0.1
// GRLighting.cpp
// It's The GreenRay Source File For Implementing Light And BumpMapping Effects.
// Date Creation: 29 August 2005
// Last Changing: 5 September 2006
// Author Rights By: Zie Technology, Zombie Soft And Lucky's Flash Studio Inc.,
// Licensed By: (C)DDD Digital Dreams Development And (C)3D Studios Inc., 2006
*******************************************************************************/
#include <GRLighting.h>
#include <GRMethods.h>
#include <GRShaders.h>

//Define A Custom Vertex
struct PANEL_VERTEX
{
  FLOAT x,y,z; //Position Of Vertex In Space 
  DWORD Color;
  FLOAT nx,ny,nz;
  FLOAT tu,tv; //Texture Coordinates

   FLOAT sx, sy, sz;
   FLOAT bx, by, bz;
};

//Define A FVF
#define PANEL_D3DFVF_VERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_NORMAL|D3DFVF_TEX1 /*| D3DFVF_XYZ|D3DFVF_XYZ*/)

LPDIRECT3DVERTEXDECLARATION9 m_pVertexDecl; //Vertex Declaration Interface
LPDIRECT3DVERTEXBUFFER9 m_pVertexBuffer; //Vertex Buffer 
LPDIRECT3DTEXTURE9 m_pTexture,m_pTexture01; //Texture
LPD3DXEFFECT m_pEffect; //Effect
D3DXHANDLE m_pTechHandle; //Effect Technique Handle

//CGRModels* pMesh; //Testing Mesh

//-----------------------------------------------------------------------------
// Name: RenderHardwareTnL()
// Desc: Render Hardware Transform And Lighting Effect (T&L)
//-----------------------------------------------------------------------------
void RenderHardwareTnL(LPDIRECT3DDEVICE9 pDevice,D3DXVECTOR3 VecNormal,int LightNumber)
{
  if (LightNumber == 0 || LightNumber == 1) 
  {
    D3DMATERIAL9 Material; //Material
    D3DLIGHT9 Light; //Light
  
    //Setting Up Material
    //                     R    G    B    A
	InitMaterial(Material,1.0f,1.0f,0.99f,1.0f);
    pDevice->SetMaterial(&Material);

    //Setup Light
    ZeroMemory(&Light,sizeof(D3DLIGHT9));
    Light.Type       = D3DLIGHT_DIRECTIONAL;
    Light.Diffuse.r  = 1.0f;
    Light.Diffuse.g  = 1.0f;
    Light.Diffuse.b  = 1.0f;
    Light.Range      = 10000.0f;
 
    //Light.Direction = D3DXVECTOR3(0.5f, -0.8f, 0.7f);
    Light.Position = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

    //Setup Normals
    D3DXVECTOR3 VectorDir;
    VectorDir = VecNormal,D3DXVec3Normalize((D3DXVECTOR3*)&Light.Direction,&VectorDir);

    //Render Light
    pDevice->SetLight(LightNumber,&Light);
    pDevice->LightEnable(LightNumber,TRUE);
    pDevice->SetRenderState(D3DRS_LIGHTING,TRUE);
    pDevice->SetRenderState(D3DRS_AMBIENT,0);
  }

  if (LightNumber == 2) 
  {
	pDevice->LightEnable(1,FALSE);
    pDevice->LightEnable(0,FALSE);
	//pDevice->SetRenderState(D3DRS_LIGHTING,FALSE);
  }

} //EndRenderHardwareTnLProcedure

//-----------------------------------------------------------------------------
// Name: CreateTexture()
// Desc: 
//-----------------------------------------------------------------------------
void CreateTexture(LPDIRECT3DDEVICE9 pDevice,TCHAR* FileName,DWORD dwKeyColor)
{
/*  if (FAILED(D3DXCreateTextureFromFileEx(pDevice,TEXT("C:\\DXDevelopment\\3D Studios\\GreenRay-3\\Release v3.1\\Debug\\relief_rockwall.jpg"),0,0,0,0,
  D3DFMT_UNKNOWN,D3DPOOL_MANAGED,D3DX_DEFAULT,D3DX_DEFAULT,dwKeyColor,NULL,NULL,&m_pTexture)))
  {
    ShowMessage("GRLighting.cpp: Can't Create User Interface Texture..");
	m_pTexture = NULL;
  }

  if (FAILED(D3DXCreateTextureFromFileEx(pDevice,TEXT("C:\\DXDevelopment\\3D Studios\\GreenRay-3\\Release v3.1\\Debug\\relief_rockwall.tga"),0,0,0,0,
  D3DFMT_UNKNOWN,D3DPOOL_MANAGED,D3DX_DEFAULT,D3DX_DEFAULT,dwKeyColor,NULL,NULL,&m_pTexture01)))
  {
    ShowMessage("GRLighting.cpp: Can't Create User Interface Texture..");
	m_pTexture = NULL;
  }

*/
	D3DXCreateTextureFromFile(pDevice,"C:\\DXDevelopment\\3D Studios\\GreenRay-3\\Release v3.1\\relief_rockwall.jpg",&m_pTexture);

	D3DXCreateTextureFromFile(pDevice,"C:\\DXDevelopment\\3D Studios\\GreenRay-3\\Release v3.1\\relief_rockwall.tga",&m_pTexture01);

} //EndCreateTextureProcedure

//-----------------------------------------------------------------------------
// Name: CreateVertexBuffer()
// Desc: 
//-----------------------------------------------------------------------------
HRESULT CreateVertexBuffer(LPDIRECT3DDEVICE9 pDevice)
{
  HRESULT _Result = S_OK;
  const FLOAT fDepth = 121.0f; //Z

  //Vertexes Description
  PANEL_VERTEX Vertexes[] = 
  {
	{-50.0f,  50.0f, fDepth, 0xffffff, 0.0f, 0.0f, 1.0f,   0.0f, 0.0f,  1, 0, 0,  0, -1, 0 }, //A
	{ 50.0f,  50.0f, fDepth, 0xffffff, 0.0f, 0.0f, 1.0f,   1.0f, 0.0f,  1, 0, 0,  0, -1, 0 }, //B
	{ 50.0f, -50.0f, fDepth, 0xffffff, 0.0f, 0.0f, 1.0f,   1.0f, 1.0f,  1, 0, 0,  0, -1, 0 }, //C

	{ 50.0f, -50.0f, fDepth, 0xffffff, 1.0f, 0.0f, 0.0f,   1.0f, 1.0f,  1, 0, 0,  0, -1, 0 }, //C
	{-50.0f, -50.0f, fDepth, 0xffffff, 1.0f, 0.0f, 0.0f,   0.0f, 1.0f,  1, 0, 0,  0, -1, 0 }, //D
	{-50.0f,  50.0f, fDepth, 0xffffff, 1.0f, 0.0f, 0.0f,   0.0f, 0.0f,  1, 0, 0,  0, -1, 0 }, //A
     //X        Y      Z     TexU  TexV
  }; //End Vertexes Description

  //Create Vertex Buffer
  if (FAILED(pDevice->CreateVertexBuffer(6*sizeof(PANEL_VERTEX),0, 
  PANEL_D3DFVF_VERTEX,D3DPOOL_DEFAULT,&m_pVertexBuffer,NULL))) _Result = E_FAIL;

  //Lock The Vertex Buffer
  VOID* pBV;
  if (FAILED(m_pVertexBuffer->Lock(0,sizeof(Vertexes),(void**)&pBV,0))) _Result = E_FAIL;
  
  memcpy(pBV,Vertexes,sizeof(Vertexes)); //Copy Vertex Data To Memory

  m_pVertexBuffer->Unlock(); //Unlock The Vertex Buffer

  return _Result;
} //EndCreateVertexBufferFunction

//-----------------------------------------------------------------------------
// Name: CreateJohnB()
// Desc: Simple Bump Technology
//-----------------------------------------------------------------------------
/*void CreateJohnB(LPDIRECT3DDEVICE9 pDevice)
{
  m_pEffect = NULL;
  LPD3DXEFFECT TempEffect = NULL;
  m_pVertexDecl = NULL;

  //Create Vertex Declaration For Shader Input Registers
  D3DVERTEXELEMENT9 Decl[] =
  {
    {0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
    {0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},

    {0, 24, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR,    0},  
    {0, 28, D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,   0},
    {0, 32, D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT,  0},
    {0, 44, D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL, 0},


		
    D3DDECL_END()
  };
  

  //Creation And Loading Simple Model Class
  pMesh = new CGRModels();
  pMesh->CreateModelFromX(pDevice,L"Keksik.x");


  //Create Vertex Declaration For This Shader Input Registers
  if (SUCCEEDED(CreateVertDeclaration(pDevice,&m_pVertexDecl,Decl)))
  {
    //Load Some Shader File
    if (SUCCEEDED(LoadShaderFile(pDevice,TEXT("\Shaders\\ReliefMapping.fx"),&TempEffect)))
    {
      m_pEffect = TempEffect;
	  m_pTechHandle = m_pEffect->GetTechniqueByName("parallax_mapping");
    }
    else 
    {
      ShowMessage("GRLighting.cpp: Unable To Load Shader File ReliefMapping.fx..");
      TempEffect = NULL;
	  m_pEffect = NULL;
    }
	
//	//Initialize Vertex Buffer
 //   if (FAILED(CreateVertexBuffer(pDevice))) 
 //   { 
 //     ShowMessage("GRLighting.cpp: Unable To Create Vertex Buffer.."); 
  //    m_pVertexBuffer = NULL;
   // }

   CreateTexture(pDevice,TEXT("\Textures\\UI\\Vitia\\Vitia.bmp"),D3DCOLOR_XRGB(0,0,0));

  }

} //End*/

//-----------------------------------------------------------------------------
// Name: RenderJohnB()
// Desc: Rndring Simple Bump Technology
//-----------------------------------------------------------------------------
/*void RenderJohnB(LPDIRECT3DDEVICE9 pDevice)
{
  //If UI Effect Is Ready Then Render UI Now
  if (m_pEffect != NULL)
  {  
    pDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_CCW);
                                                              
    //pDevice->SetStreamSource(0,m_pVertexBuffer,0,sizeof(PANEL_VERTEX));
    pDevice->SetFVF(NULL);
    pDevice->SetVertexDeclaration(m_pVertexDecl);

    m_pEffect->SetTechnique(m_pTechHandle);
    m_pEffect->SetTexture("texmap",m_pTexture);
	m_pEffect->SetTexture("reliefmap",m_pTexture01);
    

    pMesh->RotationModel(0.0f, 0.0f, 0.0f);
    pMesh->ScaleModel(0.9f, 0.9f, 0.9f);
    pMesh->DispositionModel(0.0f, 0.0f, 0.0f);

    m_pEffect->SetMatrix("modelviewproj",&CameraProjection());
    m_pEffect->SetMatrix("modelview",&CameraView());
	m_pEffect->SetMatrix("view",&pMesh->_ObjectToView);

    //Render
	UINT uPasses,uPass;
    m_pEffect->Begin(&uPasses,0);
    for (uPass = 0; uPass < uPasses; uPass++)
    {
      m_pEffect->BeginPass(uPass);  

	  //Rendering Test Panel
      //pDevice->DrawPrimitive(D3DPT_TRIANGLELIST,0,2);
	  pMesh->Render(pDevice);

      m_pEffect->EndPass();  
    }
    m_pEffect->End();
  }
} //End*/

//-----------------------------------------------------------------------------
