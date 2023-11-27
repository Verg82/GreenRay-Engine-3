/******************************************************************************
// It's Unofficial Version Of The GreenRay Engine v.3.0.0.3
// GR_Weapon.cpp
// It's The GreenRay Weapon Class Source File.
// Programming By: Verg
// Date Creation: 29 August 2005
// Last Changing: 26 August 2007
// Author Rights By: Zie Technology And Lucky's Flash Studio Inc.,
// Licensed By: (C)DDD Digital Dreams Development Inc., 2007
******************************************************************************/
#include <GR_Weapon.h>

////////////////////////////////////////////////////////////////////////////////
// External Definitions And Prototypes 
////////////////////////////////////////////////////////////////////////////////
//Vertex Declarations For Another X-Files Converted From 3DS Max
D3DVERTEXELEMENT9 g_pVertDecl_[] =
{
  {0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
  {0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,   0},
  {0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
  //{0, 32, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT,  0},
  //{0, 44, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL, 0},	  
  D3DDECL_END()
};

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction For CGRWeapon Class
///////////////////////////////////////////////////////////////////////////////
CGRWeapon::CGRWeapon(LPDIRECT3DDEVICE9 pDevice)
{
  d_pDevice = pDevice; //Apropritate Device Pointer To Class
  m_pWeapon = NULL;
  m_pHUDBullets = NULL; 
  pVertDecl = NULL;
 
  Be_Shoot = false;
  Num_Bullets = 200; //Give Bullet Into Your Weapon Here
  Inc_Clicks = 0;

  pVertexBuffer = NULL;
  m_nWidth = 1.0f;
  m_nHeight = 1.0f;
  dwColor = D3DCOLOR_XRGB(255,255,255);

  //Create Gun Fire Texture
  if (FAILED(D3DXCreateTextureFromFileEx(d_pDevice,"\Textures\\Gun_Fire\\MFlash.dds", 0,0,0,0,
  D3DFMT_UNKNOWN,D3DPOOL_MANAGED,D3DX_DEFAULT,D3DX_DEFAULT,dwColor,NULL,NULL,&m_pTexture)))
  {
    ShowMessage("GR_Weapon.cpp: Can't Create Interface Texture..");
  }

  //Initialize Vertex Buffer For Out Fire Billboard
  if (FAILED(CreateVertexBuffer())) ShowMessage("GR_Weapon.cpp: Unable To Create Vertex Buffer..");

  //Create Weapon-Model To Represent
  {
    m_pWeapon = new CGRModel(); //Construct A Weapon Model Class
    //if (m_pTexPath != NULL) 
	m_pWeapon->SetTexturePath("\Textures\\Weapons\\Beretta\\"); //Set Model Texture Path

    //Create Vertex Declaration
    d_pDevice->CreateVertexDeclaration(g_pVertDecl_,&pVertDecl);
 
    //Load An Object To Cast
    if (FAILED(m_pWeapon->Create(d_pDevice,L"\Models\\Weapons\\Beretta\\Beretta.x")))
    {
      ShowMessage("GR_Weapon.cpp: Error Load Weapon Model..");
    }

    //Set A Reasonable Vertex Type
    m_pWeapon->SetVertexDecl(g_pVertDecl_);
    //m_pWeapon->SetFVF(d_pDevice,MODEL_VERTEX_FVF);  

    //Initialize The Vertex Buffers For The File-Based Objects
    m_pWeapon->RestoreObjects(); 
  } //End Create Weapon-Model

  //Create Weapon Shoot Sound
  m_pShoot = new TGRWave();
  m_pShoot->CreateSoundInterface(FormL_HWnd,TEXT("\Sounds\\Weapon\\Beretta\\BerettaFire.wav"),2,44100,32);
  //Create Weapon Empty Sound
  m_pEmpty = new TGRWave();
  m_pEmpty->CreateSoundInterface(FormL_HWnd,TEXT("\Sounds\\Weapon\\Beretta\\Empty.wav"),2,44100,32);
  //Create Vitia Sound
  m_pVitia = new TGRWave();
  m_pVitia->CreateSoundInterface(FormL_HWnd,TEXT("\Sounds\\Vitia\\Speech\\Kogo Tam.wav"),2,44100,32);

  //Create Weapon Bullet Counter Label Parameters
  m_pHUDBullets = new CGRLabel(d_pDevice); //Construct A Label
  m_pHUDBullets->FontColor = clYellow; //Set Font Color
  m_pHUDBullets->FontName = "Arial"; //Set Font Name
  m_pHUDBullets->FontSize = 38; //Set Font Size
  m_pHUDBullets->FontStyle = FS_BETA; //Set Font Style
  m_pHUDBullets->Left = 60;
  m_pHUDBullets->Top = ScreenHeight-58-6; 
  m_pHUDBullets->Update(); //Update Label Parameters

  //Create Weapon Information Label
  m_pInfo = new CGRLabel(d_pDevice); 
  m_pInfo->FontColor = clYellow; 
  m_pInfo->FontName = "Arial"; 
  m_pInfo->FontSize = 10; 
  m_pInfo->FontStyle = FS_BETA; 
  m_pInfo->Left = 21;
  m_pInfo->Top = ScreenHeight-58+27; 
  m_pInfo->Update(); 
} //EndConstructionDirectives

CGRWeapon::~CGRWeapon()
{ 
} //EndDestructionDirectives

//-----------------------------------------------------------------------------
// Name: CGRWeapon::Release()
// Desc: 
//-----------------------------------------------------------------------------
void CGRWeapon::Release()
{
  //ReleaseClass(pTexture);
  //ReleaseClass(pVertexBuffer);
  
  //Free Weapon Model
  if (m_pWeapon != NULL) m_pWeapon = NULL;

  //Free Weapon Shoot Sound
  if (m_pShoot != NULL) { m_pShoot->ReleaseSound(); m_pShoot = NULL; }
  //Free Weapon Empty Sound
  if (m_pEmpty != NULL) { m_pEmpty->ReleaseSound(); m_pEmpty = NULL; }
  //Free Vitia Sound
  if (m_pVitia != NULL) { m_pVitia->ReleaseSound(); m_pVitia = NULL; }

  //Free Information Labels
  if (m_pHUDBullets != NULL) { m_pHUDBullets->Release(); m_pHUDBullets = NULL; }
  if (m_pInfo != NULL) { m_pInfo->Release(); m_pInfo = NULL; }
} //EndReleaseProcedure

//-----------------------------------------------------------------------------
// Name: CGRWeapon::SetTexturePath()
// Desc: This Procedure For Setting Path Where Texture There Found
//-----------------------------------------------------------------------------
void CGRWeapon::SetTexturePath(LPCSTR Tex_Path)
{
  m_pTexPath = Tex_Path;
} //EndSetTexturePathProcedure

//-----------------------------------------------------------------------------
// Name: CGRWeapon::CreateVertexBuffer()
// Desc: 
//-----------------------------------------------------------------------------
HRESULT CGRWeapon::CreateVertexBuffer()
{
  HRESULT _Result = S_OK;

  const FLOAT fDepth = 1.0f; //Z

  //Vertexes Description
  BILLBOARD_VERTEX Vertices[] = 
  {
	{0.0,      m_nHeight, fDepth, dwColor, 0.0f, 0.0f}, //A
	{m_nWidth, m_nHeight, fDepth, dwColor, 1.0f, 0.0f}, //B
	{m_nWidth, 0.0f,      fDepth, dwColor, 1.0f, 1.0f}, //C

	{m_nWidth, 0.0f,      fDepth, dwColor, 1.0f, 1.0f}, //C
	{0.0f,     0.0f,      fDepth, dwColor, 0.0f, 1.0f}, //D
	{0.0f,     m_nHeight, fDepth, dwColor, 0.0f, 0.0f}, //A
     //X         Y           Z     Color    TexU  TexV
  }; //End Vertices Description

  //Create Vertex Buffer
  if (FAILED(d_pDevice->CreateVertexBuffer(6*sizeof(BILLBOARD_VERTEX),0, 
  BILLBOARD_D3DFVF_VERTEX,D3DPOOL_DEFAULT,&pVertexBuffer,NULL))) _Result = E_FAIL;

  //Lock The Vertex Buffer
  VOID* pBV;
  if (FAILED(pVertexBuffer->Lock(0,sizeof(Vertices),(void**)&pBV,0))) _Result = E_FAIL;
  
  memcpy(pBV,Vertices,sizeof(Vertices)); //Copy Vertex Data To Memory

  pVertexBuffer->Unlock(); //Unlock The Vertex Buffer

  return _Result;
} //EndCreateVertexBufferFunction

//-----------------------------------------------------------------------------
// Name: CGRWeapon::Render()
// Desc: 
//-----------------------------------------------------------------------------
void CGRWeapon::Render()
{
  //Setting ViewPort Variables And Parameters For Rendering Weapon Model
  D3DSURFACE_DESC m_pSurface;
  D3DVIEWPORT9 ViewPort = {0, 0, 0, 0, 0.0f, 0.1f};
  D3DVIEWPORT9 OriginalVP;
  IDirect3DSurface9* pBackBuffer = NULL;

  d_pDevice->GetRenderTarget(0,&pBackBuffer);
  pBackBuffer->GetDesc(&m_pSurface);
  
  //Calculate The Viewport  
  ViewPort.Width = 1024;
  ViewPort.Height = 768;
                                                               
  if (ViewPort.Width > m_pSurface.Width) ViewPort.Width = m_pSurface.Width; 
  if (ViewPort.Height > m_pSurface.Height) ViewPort.Height = m_pSurface.Height;
  
  ViewPort.X = 0;
  ViewPort.Y = 0;

  //Prepare The Viewport For Rendering
  d_pDevice->GetViewport(&OriginalVP);
  d_pDevice->SetViewport(&ViewPort);

  //Render Some Weapon Model
  {
    //Render State For Until Rendering Mesh Object
    d_pDevice->SetRenderState(D3DRS_ZENABLE,TRUE);
    d_pDevice->SetRenderState(D3DRS_ZWRITEENABLE,TRUE);
    d_pDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_CCW);
    d_pDevice->SetRenderState(D3DRS_LIGHTING,TRUE);

    //VU Addr
    d_pDevice->SetSamplerState(0,D3DSAMP_ADDRESSU,D3DTADDRESS_WRAP);
    d_pDevice->SetSamplerState(0,D3DSAMP_ADDRESSV,D3DTADDRESS_WRAP);

    //Set Texturing State Parameters
    d_pDevice->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR);
    d_pDevice->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_LINEAR);
    
    //Anisotropic Texture Filtering
    //d_pDevice->SetSamplerState(0,D3DSAMP_MAXANISOTROPY,4);
    //d_pDevice->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_ANISOTROPIC);
    //d_pDevice->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_ANISOTROPIC);
    //d_pDevice->SetSamplerState(0,D3DSAMP_MIPFILTER,D3DTEXF_ANISOTROPIC);
	
	//Set Perspective View For The Weapon Model In Space
    float Aspect_Ratio = 1.1f;
    D3DXMATRIXA16 _MatProj;
    D3DXMatrixPerspectiveFovLH(&_MatProj,D3DX_PI/4,Aspect_Ratio,0.1f,1000.0f);
    d_pDevice->SetTransform(D3DTS_PROJECTION,&_MatProj);

    //Translation Matrices For The Weapon Model Object
    D3DXMATRIXA16 _MatView;
    D3DXMatrixTranslation(&_MatView, 0.0f, 0.0f, 0.0f);
    d_pDevice->SetTransform(D3DTS_VIEW,&_MatView);

    //Translation Matrices For The Object
    D3DXMATRIXA16 _MatTrans,Pos_Matrix,Scale_Matrix; //,_MatCentered;
    //                                        W           H           D
    //D3DXMatrixTranslation(&_MatCentered,-m_pWidth/2,-m_pHeight/2,-m_pDepth/2);
	//D3DXMatrixTranslation(&Pos_Matrix,1.0f,-1.0f,4.0f);
   
    D3DXMatrixIdentity(&Pos_Matrix);
	Pos_Matrix._41 = 1.0f;
    Pos_Matrix._42 = -1.0f;
    Pos_Matrix._43 = 4.0f;
	
	//D3DXMatrixScaling(&Scale_Matrix,0.1f,0.1f,0.1f);

    _MatTrans = (Pos_Matrix); //*Scale_Matrix;
    d_pDevice->SetTransform(D3DTS_WORLD,&_MatTrans); //Transform Object Position In Space

    //Rendering Model Here
    LPD3DXMESH m_pMesh = m_pWeapon->GetLocalMesh();
    
    for (DWORD I = 0; I < /*m_pWeapon->m_dwNumMaterials*/ 11; ++I)
    {
      //Set Material And Texture For The Mesh Object
      //pDevice->SetMaterial(&m_pWeapon->m_pMaterials[I]);
	  d_pDevice->SetTexture(0,m_pWeapon->m_pTextures[I]);
      m_pMesh->DrawSubset(I);
    }
    //if (m_pWeapon != NULL) m_pWeapon->Render();
  


    if (Be_Shoot == true) //Render Gun Shoot Fire
    {
      //Setting Rendering Parameters
      d_pDevice->SetRenderState(D3DRS_LIGHTING,FALSE);
  
      //Enable Alpha Blending So We Can Use Transparent Textures
      d_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);

      //Set How The Texture Should Be Blended (Use Alpha) 
      d_pDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
      d_pDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
  
      //Translation Matrices For This Billboard Object  
//    D3DXMATRIXA16 Pos_Matrix; 
//    D3DXMatrixTranslation(&Pos_Matrix,1.09f,-1.89f,8.0f); 
//    d_pDevice->SetTransform(D3DTS_WORLD,&Pos_Matrix); //Transform Position In Space
  
      //Render Some Billboard
//    d_pDevice->SetStreamSource(0,pVertexBuffer,0,sizeof(BILLBOARD_VERTEX));
//    d_pDevice->SetFVF(BILLBOARD_D3DFVF_VERTEX);

      d_pDevice->SetTexture(0,m_pTexture);
      d_pDevice->SetTextureStageState(0,D3DTSS_ALPHAOP,D3DTOP_MODULATE);
      //d_pDevice->DrawPrimitive(D3DPT_TRIANGLELIST,0,2);

      //Render Flame
      for (DWORD I = 11; I < 12; ++I)
      {
        //Set Material And Texture For The Mesh Object
        //pDevice->SetMaterial(&pMesh->m_pMaterials[I]);
	    //pDevice->SetTexture(0,pMesh->m_pTextures[I]);
        m_pMesh->DrawSubset(I);
      }
	  Be_Shoot = false;   
    }
  } //End Render Weapon Model
  
  //Disable Alpha Blending
  d_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,FALSE);

  //When Render Passed Then Release Viewports 
  ReleaseClass(pBackBuffer);
  d_pDevice->SetViewport(&OriginalVP);

  //Output Bullet Count
  const int Fmt_Str = sizeof(m_pBullStr)/sizeof(TCHAR);
  if (Num_Bullets >= 100) _sntprintf(m_pBullStr,Fmt_Str,_T("%d"),Num_Bullets,0,0,0,0,0);
  if (Num_Bullets < 100) _sntprintf(m_pBullStr,Fmt_Str,_T("  %d"),Num_Bullets,0,0,0,0,0);
  if (Num_Bullets < 10) _sntprintf(m_pBullStr,Fmt_Str,_T("   %d"),Num_Bullets,0,0,0,0,0);
  m_pBullStr[Fmt_Str-1] = TEXT('\0');

  //Render Captions
  m_pHUDBullets->Caption(m_pBullStr);
  m_pInfo->Caption("Боевые");
} //EndRenderProcedure

//-----------------------------------------------------------------------------
// Name: CGRWeapon::WeaponShoot()
// Desc: 
//-----------------------------------------------------------------------------
void CGRWeapon::WeaponShoot()
{
  if (Num_Bullets == 0)
  {
    if (m_pEmpty != NULL) m_pEmpty->PlaybackReset();
	if (Inc_Clicks == 2) 
	{
	  //if (m_pVitia != NULL) m_pVitia->PlaybackSound();
	}
	if (Inc_Clicks <= 3) Inc_Clicks++;
  }
  else
  {
    Num_Bullets--;
    if (m_pShoot != NULL) m_pShoot->PlaybackReset();
	Be_Shoot = true;
  }
} //EndWeaponShootProcedure