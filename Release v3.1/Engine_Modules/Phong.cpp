/******************************************************************************
// It's Unofficial Version Of The GreenRay Engine v.1.0.0.3
// GR_Weapon.cpp
// It's The GreenRay Weapon Class Source File.
// Date Creation: 29 August 2005
// Last Changing: 23 February 2007
// Author Rights By: Zie Technology, Zombie Soft And Lucky's Flash Studio Inc.,
// Licensed By: (C)DDD Digital Dreams Development Inc., 2007
******************************************************************************/
#include <GR_Weapon.h>

///////////////////////////////////////////////////////////////////////////////
// Defines And Constants
///////////////////////////////////////////////////////////////////////////////
#define NUM_LIGHTS 3 //Number Of Lights In The Scene
#define EMISSIVE_COEFFICIENT 39.78f //Emissive Color Multiplier For Each Lumen Of Light Intensity 

//Shader Light Methods
D3DXVECTOR4 g_avLightPosition[NUM_LIGHTS]; //Light Positions In World Space
D3DXVECTOR4 g_avLightIntensity[NUM_LIGHTS]; //Light Floating Point Intensities
int g_nLightLogIntensity[NUM_LIGHTS]; //Light Intensities On A Log Scale
int g_nLightMantissa[NUM_LIGHTS]; //Mantissa Of The Light Intensity

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction For CGRWeapon Class
///////////////////////////////////////////////////////////////////////////////
CGRWeapon::CGRWeapon(LPDIRECT3DDEVICE9 pDevice)
{
  d_pDevice = pDevice; //Apropritate Device Pointer To Class
  m_pEffect = NULL;
  m_pTechnique = NULL;

  //Create A PhongLight Shader Class
 // CreateShader(TEXT("\Shaders\\PhongLighting.fx"));

  //Create Weapon-Model To Represent
  m_pWeapon = new CGRModels(); //Construct A Weapon Model Class
  m_pWeapon->SetTexturePath("\Textures\\Weapons\\SVD\\");
  m_pWeapon->CreateModelFromX(d_pDevice,L"\Models\\Weapons\\SVD\\SVD.x");

  
  //Set Light Positions In World Space And Light Intensity
/*  g_avLightPosition[0] = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f);
  g_nLightMantissa[0] = 6.5f;
  g_nLightLogIntensity[0] = 6.5f;

  #if NUM_LIGHTS > 1
  g_avLightPosition[1] = D3DXVECTOR4(1.0f, 2.5f, 16.0f, 1.0f);
  g_nLightMantissa[1] = 6.5f;
  g_nLightLogIntensity[1] = 6.5f;
  #endif
 
  #if NUM_LIGHTS > 2
  g_avLightPosition[2] = D3DXVECTOR4(14.0f, 2.5f, 10.0f, 1.0f);
  g_nLightMantissa[2] = 6.5f;
  g_nLightLogIntensity[2] = 6.5f;
  #endif

  #if NUM_LIGHTS > 3
  g_avLightPosition[3] = D3DXVECTOR4(14.0f, 2.5f, 16.0f, 1.0f);
  g_nLightMantissa[3] = 6.5f;
  g_nLightLogIntensity[3] = 6.5f;
  #endif

  RefreshLights(); //Refresh Light Parameters*/
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
//  ReleaseClass(m_pEffect);
//  m_pTechnique = NULL;
} //EndReleaseProcedure

//-----------------------------------------------------------------------------
// Name: CGRWeapon::RefreshLights()
// Desc: Set The Light Intensities To Match The Current Log Luminance
//-----------------------------------------------------------------------------
void CGRWeapon::RefreshLights()
{
  for (int I = 0; I < NUM_LIGHTS; I++)
  {
    g_avLightIntensity[I].x = g_nLightMantissa[I]*(float)pow(1.0f,g_nLightLogIntensity[I]);
    g_avLightIntensity[I].y = g_nLightMantissa[I]*(float)pow(1.0f,g_nLightLogIntensity[I]);
    g_avLightIntensity[I].z = g_nLightMantissa[I]*(float)pow(1.0f,g_nLightLogIntensity[I]);
    g_avLightIntensity[I].w = 1.0f;          
  }
} //EndRefreshLightsProcedure

//-----------------------------------------------------------------------------
// Name: CGRWeapon::CreateShader()
// Desc: 
//-----------------------------------------------------------------------------
void CGRWeapon::CreateShader(TCHAR* EffectFile)
{
  //Read The D3DX Effect File
  //If This Fails, There Should Be Debug Output As To They The .fx File Failed To Compile
  //Set Of Three Flags
  //DWORD dwShaderFlags = D3DXFX_NOT_CLONEABLE;
  //dwShaderFlags |= D3DXSHADER_FORCE_VS_SOFTWARE_NOOPT;
  //dwShaderFlags |= D3DXSHADER_FORCE_PS_SOFTWARE_NOOPT;

   //Check On PixelShader 2.0 Present Parameters
  D3DCAPS9 _Caps;
  d_pDevice->GetDeviceCaps(&_Caps);
  if (FAILED(_Caps.PixelShaderVersion < D3DPS_VERSION(2,0)))
  {
    ShowMessage("GR_Weapon.cpp: Your Video Card Not Support Pixel Shader Version 2.0..");
  }
  else 
  {
    //Creation Effect
    if (FAILED(D3DXCreateEffectFromFile(d_pDevice,EffectFile,
    NULL,NULL,D3DXFX_NOT_CLONEABLE,NULL,&m_pEffect,NULL)))
    {
      ShowMessage("GR_Weapon.cpp: Error Load Shader Effect File..");
	  ReleaseClass(m_pEffect);
	  return;
    } //EndCreationEffect

    //Set Effect File Variables For Effective Methods
    m_pTechnique = m_pEffect->GetTechniqueByName("RenderScene");  

    //SetTechnique
    if (FAILED(m_pEffect->SetTechnique(m_pTechnique)))
    {
	  ShowMessage("GR_Weapon.cpp: Can't Set Technigue..");
	  ReleaseClass(m_pEffect);
      m_pTechnique = NULL;
	  return;
    } //EndSetTechnique
  }
} //EndCreateShaderProcedure

//-----------------------------------------------------------------------------
// Name: CGRWeapon::Render()
// Desc: 
//-----------------------------------------------------------------------------
void CGRWeapon::Render()
{
  //Setting Rendering Parameters
  d_pDevice->SetRenderState(D3DRS_ZENABLE,TRUE);
  d_pDevice->SetRenderState(D3DRS_ZWRITEENABLE,TRUE);
  d_pDevice->SetRenderState(D3DRS_LIGHTING,TRUE);

  //Setting Shader Parameters
/*  m_pEffect->SetMatrix("g_mProjection",&gr_Camera->Projection); //Set Projection Matrix For Our Effect !!!

  //Calculate The Position Of The Lights In View Space
  D3DXVECTOR4 avLightViewPosition[NUM_LIGHTS]; 
  
  for (int iLight = 0; iLight < NUM_LIGHTS; iLight++)
  {
	D3DXVec4Transform(&avLightViewPosition[iLight],&g_avLightPosition[iLight],&gr_Camera->View);
  }

  //Set Frame Shader Constants
  m_pEffect->SetValue("g_avLightPositionView",avLightViewPosition,sizeof(D3DXVECTOR4)*NUM_LIGHTS);
  m_pEffect->SetValue("g_avLightIntensity",g_avLightIntensity,sizeof(D3DXVECTOR4)*NUM_LIGHTS);

  UINT uiPassCount,uiPass;

  d_pDevice->SetSamplerState(0,D3DSAMP_ADDRESSU,D3DTADDRESS_WRAP);
  d_pDevice->SetSamplerState(0,D3DSAMP_ADDRESSV,D3DTADDRESS_WRAP);
  
  m_pEffect->SetTechnique(m_pTechnique);
  m_pEffect->SetMatrix("g_mObjectToView",&gr_Camera->View);
   
  m_pEffect->Begin(&uiPassCount,0);
  for (uiPass = 0; uiPass < uiPassCount; uiPass++)
  {
    m_pEffect->BeginPass(uiPass);

    //Turn Off Emissive Lighting
    D3DXVECTOR4 vNull(0.0f, 0.0f, 0.0f, 0.0f);
    m_pEffect->SetVector("g_vEmissive",&vNull); 
    
    //Enable Texturing
    m_pEffect->SetBool("g_bEnableTexture",true);
    //d_pDevice->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR);
    //d_pDevice->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_LINEAR);
    
    //Anisotropic Texture Filtering
    d_pDevice->SetSamplerState(0,D3DSAMP_MAXANISOTROPY,4);
    d_pDevice->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_ANISOTROPIC);
    d_pDevice->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_ANISOTROPIC);
    d_pDevice->SetSamplerState(0,D3DSAMP_MIPFILTER,D3DTEXF_ANISOTROPIC);

 
    //Draw Weapon
    m_pEffect->SetFloat("g_fPhongExponent", 3.0f);
    m_pEffect->SetFloat("g_fPhongCoefficient", 0.1f);
    m_pEffect->SetFloat("g_fDiffuseCoefficient", 0.25f);
    m_pEffect->SetBool("g_bEnableTexture",true);

    //Set Transform,Scale And Rotation Matrices
	m_pWeapon->RotationModel(-1.5f, 0.0f, 0.0f);
    m_pWeapon->ScaleModel(0.0090f, 0.0090f, 0.0090f);
    m_pWeapon->DispositionModel(0.0f, 2.5f, 4.0f);
   // m_pEffect->SetMatrix("g_mObjectToView",&m_pWeapon->_ObjectToView);

	m_pEffect->CommitChanges();
	
    
	
    m_pEffect->EndPass();
  }
  m_pEffect->End();  */

  //Translation Matrices For The Model Object
  D3DXMATRIXA16 _MatView;
  D3DXMatrixTranslation(&_MatView, 0.0f, 0.0f, 0.0f);
  d_pDevice->SetTransform(D3DTS_VIEW,&_MatView);

//  m_pWeapon->Render(d_pDevice,0.1f);
} //EndRenderProcedure