/****************************************************************************/ 
// It's Unofficial Version Of The GreenRay Engine v.3.0.0.2
// GREngine.cpp Visual Studio 8 Version.
// It's An GreenRay GREngine Source File.
// Made Specially For Implement 3D Engine Parameters.
// Date Creation: 29 August 2005
// Last Changing: 21 January 2007
// Licensed By: (C)DDD Digital Dreams Development And (C)3D Studios Inc., 2007
/*****************************************************************************/
//#pragma comment(lib,"d3d9.lib");
//#pragma comment(lib,"d3dx9.lib");
#include <GREngine.h>
#include <GRMethods.h>
#include <GR_Label.h>
#include <GR_Camera.h>
#include <GR_Physics.h>
#include <GR_RayCast.h>
#include <GR_Decals.h>
#include <GR_Weapon.h>
#include <GRInterface.h>
#include <GR_World.h>
#include <GRLighting.h>
#include <GRSound.h>
#include <GR_SoundEffects.h>

#ifdef __cplusplus
#define export extern "C++" __declspec(dllexport)
#else
#define export __declspec(dllexport)
#endif

#ifdef __cplusplus
#define import extern "C++" __declspec(dllimport)
#else
#define import __declspec(dllimport)
#endif

////////////////////////////////////////////////////////////////////////////////
// Exporting External Definitions And Prototypes 
////////////////////////////////////////////////////////////////////////////////
//export HRESULT CreateDirect3D(HWND _HWnd);
//export void RenderingDirect3D();
//export void ReleaseDirect3D();

////////////////////////////////////////////////////////////////////////////////
// Global Variables 
////////////////////////////////////////////////////////////////////////////////
IDirect3DDevice9* m_pDevice = NULL; //This General Direct3D Device Interface
int ScreenWidth,ScreenHeight; //BackBuffer Screen Width And Height
HWND FormL_HWnd; //Form Layer Handle
int Inc_Song = 0;

//GreenRay Classes Globals
CGRFPSCounter* gr_FPS = NULL; //GreenRay FPS Counter Global Interface Class
CGRDeltaTimer* gr_Timer = NULL; //GreenRay Timer Interface Class
CGRCamera* gr_Camera = NULL; //GreenRay Camera Global Interface Class
CGRPhysics* gr_PhysX = NULL; //Global Physics Interface Class
CGRSkyBox* gr_SkyBox = NULL; //Global SkyBox Interface Class
CGRMusic* gr_Music = NULL; //Global Mp3 Music Interface Class
CGRSoundEffects* gr_SoundsEff = NULL; //Engine Sounds Effect Class

CGRInterface* gr_FPSUI = NULL;
CGRInterface* gr_Crosshair = NULL;
CGRInterface* gr_Console = NULL;
CGRInterface* gr_HUD01 = NULL;
CGRInterface* gr_HUD02 = NULL;
CGRInterface* gr_HUD03 = NULL;
CGRInterface* gr_Copyright02 = NULL;
CGRInterface* Vitia_Photo = NULL;
CGRInterface* Vitia_Say = NULL;

CGRLabel* Vitia_Caption = NULL; //Gandon Caption
CGRLabel* Debug_Caption = NULL; //Debug Caption
CGRLabel* Vitia_Phr1 = NULL; //Vitia's Phrase-1
CGRLabel* Vitia_Phr2 = NULL; //Vitia's Phrase-2

//Simple Map Location
CGRMap* gr_Map = NULL; //Simple Map Model Class

//Player Weapon
CGRWeapon* gr_Weapon = NULL; //Weapon Class

//-----------------------------------------------------------------------------
// Name: CreateDirect3D()
// Desc: This Function Create General Direct3D Interface And Preload 
// Setting Game Video Parameters
//-----------------------------------------------------------------------------
HRESULT CreateDirect3D(HWND _HWnd)
{
  HRESULT m_pResult = S_OK; //Returned Result Of This Function
  IDirect3D9* m_pDirect3D = NULL; //This General Direct3D Initialization Interface

  //Create Base Direct3D Interface
  m_pDirect3D = Direct3DCreate9(D3D_SDK_VERSION);

  //Check If Error Then Release Direct3D Interface
  if (!m_pDirect3D)
  {
    if (m_pDirect3D != NULL) m_pDirect3D->Release();
	ShowMessage("GREngine.cpp: Failed To Create Direct3D Interface..");
    m_pResult = E_FAIL;
  }

  //Check On Adapter Display Modes
  D3DDISPLAYMODE m_pDisplay; 
  if (FAILED(m_pDirect3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT,&m_pDisplay)))
  {
    if (m_pDirect3D != NULL) m_pDirect3D->Release();
	ShowMessage("GREngine.cpp: Failed Getting Adapter Display Mode..");
    m_pResult = E_FAIL;
  }

  //Checking On Device Format
  HRESULT _HResult;
  if (FAILED(_HResult = m_pDirect3D->CheckDeviceFormat(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL, 
  m_pDisplay.Format,D3DUSAGE_DEPTHSTENCIL,D3DRTYPE_SURFACE,D3DFMT_D16)))
  {
    if (_HResult == D3DERR_NOTAVAILABLE)
	{
	  if (m_pDirect3D != NULL) m_pDirect3D->Release();
      ShowMessage("GREngine.cpp: Attention! Stencil Format D3DFMT_D16 Not Available Current Videocard");
	  m_pResult = E_FAIL;
	}
  }

  //Checking On Device Caps Parameters
  D3DCAPS9 m_pDeviceCaps;
  if (FAILED(m_pDirect3D->GetDeviceCaps(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,&m_pDeviceCaps)))
  {
    if (m_pDirect3D != NULL) m_pDirect3D->Release();
    ShowMessage("GREngine.cpp: Failed To Get Direct3D Device Caps Parameters");
    m_pResult = E_FAIL;
  }

  DWORD _BehaviorFlags = 0;
  //Use Hardware Vertex Processing If Supported, Otherwise Default To Software 
  if (m_pDeviceCaps.VertexProcessingCaps != 0) _BehaviorFlags |= D3DCREATE_HARDWARE_VERTEXPROCESSING;
  else _BehaviorFlags |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;

  //All System Checks Passed, Create The Direct3D Device
  D3DPRESENT_PARAMETERS m_pD3DParam;
  memset(&m_pD3DParam,0,sizeof(m_pD3DParam));

  //Setup Direct3D Device Parameters 
  m_pD3DParam.SwapEffect                 = D3DSWAPEFFECT_DISCARD; //D3DSWAPEFFECT_COPY;
  m_pD3DParam.BackBufferWidth            = 1024; //Setting Up Resolution
  m_pD3DParam.BackBufferHeight           = 768;
  m_pD3DParam.Windowed                   = FALSE; //True/False States For Window Or Fullscreen Modes
  m_pD3DParam.BackBufferFormat           = m_pDisplay.Format; //D3DFMT_A8R8G8B8  
  //m_pD3DParam.FullScreen_RefreshRateInHz = m_pDisplay.RefreshRate; //D3DPRESENT_RATE_DEFAULT; 
  m_pD3DParam.EnableAutoDepthStencil     = TRUE;
  m_pD3DParam.AutoDepthStencilFormat     = D3DFMT_D16; //D3DFMT_D16 D3DFMT_D15S1 D3DFMT_D24X8
                                                       //D3DFMT_D24S8 D3DFMT_D24X4S4 D3DFMT_D32
  m_pD3DParam.BackBufferCount            = 2;
  m_pD3DParam.MultiSampleType            = D3DMULTISAMPLE_4_SAMPLES; //4_SAMPLES //For Antialiasing Mode On 4x
  //m_pD3DParam.MultiSampleQuality       = 1;
  //m_pD3DParam.Flags                    = D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL; //|D3DPRESENTFLAG_LOCKABLE_BACKBUFFER; //Depth Stencil Flags For ShadowVolume
  //m_pD3DParam.PresentationInterval       = D3DPRESENT_INTERVAL_ONE; //For Video Card Hardware Mode
  m_pD3DParam.PresentationInterval     = D3DPRESENT_INTERVAL_IMMEDIATE; //For Immediate + CPU Interval Mode

  //Get ScreenWidth And ScreenHeight
  ScreenWidth = m_pD3DParam.BackBufferWidth;
  ScreenHeight = m_pD3DParam.BackBufferHeight;
  FormL_HWnd = _HWnd; //Set Layer Window Handle

  //Create Some Direct3D Device!
  if (FAILED(m_pDirect3D->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,_HWnd,_BehaviorFlags,&m_pD3DParam,&m_pDevice)))
  {
    if (m_pDirect3D != NULL) m_pDirect3D->Release();
	ShowMessage("GREngine.cpp: Error Creating Direct3D Device..");
    m_pResult = E_FAIL;
  }

  //Ending Direct3D Presets Parameters
  if (m_pDevice != NULL)
  {
	//Set Rendering States
	m_pDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_CW); //Set Cull Mode
    m_pDevice->SetRenderState(D3DRS_ZENABLE,D3DZB_TRUE); //Enable Z-Buffer
    m_pDevice->SetRenderState(D3DRS_LIGHTING,FALSE); //Turn Off The Lighting Mode
    //m_pDevice->SetRenderState(D3DRS_FILLMODE,D3DFILL_WIREFRAME); //Setting Fill Mode
    //m_pDevice->SetRenderState(D3DRS_AMBIENT,0xFFFFFFFF); //Set Around Light
    //m_pDevice->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS,TRUE); //Enable Antialiasing Mode
    //m_pDevice->SetRenderState(D3DRS_SHADEMODE,D3DSHADE_GOURAUD); //Set Shade Mode 

    //Loading Needed Directives..
    gr_FPS = new CGRFPSCounter(m_pDevice); //Create FPS Counter Class

    //Create FPS UI Layer Class
	gr_FPSUI = new CGRInterface(m_pDevice,1,2,84,20, 50); //Create FPS UI Layer Element
    gr_FPSUI->CreateTexture(TEXT("\Textures\\UI\\FPS_Layer\\FPS_Layer.bmp"),D3DCOLOR_XRGB(1,0,0)); //Create Texture For FPSUI

	//Create Crosshair Class
    gr_Crosshair = new CGRInterface(m_pDevice,((ScreenHeight-31)*0.5),((ScreenWidth-31)*0.5),31,31, 100); //Create Crosshair
    gr_Crosshair->CreateTexture(TEXT("\Textures\\UI\\Crosshairs\\Cross_03Wide.bmp"),D3DCOLOR_XRGB(0,0,0)); //Create Crosshair Texture

	//Create Console Class
	gr_Console = new CGRInterface(m_pDevice,0,(ScreenWidth-594-3),594,510, 48); //Create Console
	gr_Console->CreateTexture(TEXT("\Textures\\UI\\Console\\Console_01.bmp"),D3DCOLOR_XRGB(0,0,0)); //Create Console Texture
   
	//Create Copyright Classes
    gr_HUD01 = new CGRInterface(m_pDevice,(ScreenHeight-58-7),7,148,58, 65); //Create HUD_01
    gr_HUD01->CreateTexture(TEXT("\Textures\\UI\\HUD\\HUD_01.bmp"),D3DCOLOR_XRGB(0,0,0)); //Create HUD_01 Texture

    //Create Gandon Console Class
    gr_HUD02 = new CGRInterface(m_pDevice,28,2,408,129, 45); //Create HUD_02
    gr_HUD02->CreateTexture(TEXT("\Textures\\UI\\HUD\\HUD_02.bmp"),D3DCOLOR_XRGB(0,0,0)); //Create HUD_02 Texture

    //Create Gandon Photo
    Vitia_Photo = new CGRInterface(m_pDevice,54,10,100,95, 95); //Create Vitia_Photo
    Vitia_Photo->CreateTexture(TEXT("\Textures\\UI\\Vitia\\VitiaBlack.bmp"),D3DCOLOR_XRGB(0,0,0)); //Create Vitia_Photo Texture

    //Create Gandon Say Panel
    Vitia_Say = new CGRInterface(m_pDevice,48,109,288,79, 95); //Create Vitia_Say Panel
    Vitia_Say->CreateTexture(TEXT("\Textures\\UI\\Vitia\\VitiaSay.bmp"),D3DCOLOR_XRGB(0,0,0)); //Create Vitia_Say Texture

    //Create Debug Console Class
    gr_HUD03 = new CGRInterface(m_pDevice,28,728,288,476, 45); //Create HUD_03
    gr_HUD03->CreateTexture(TEXT("\Textures\\UI\\HUD\\HUD_03Vertical.bmp"),D3DCOLOR_XRGB(0,0,0)); //Create HUD_03 Texture

    gr_Copyright02 = new CGRInterface(m_pDevice,(ScreenHeight-57),(ScreenWidth-228),228,57, 75); //Create Copyright02
	gr_Copyright02->CreateTexture(TEXT("\Textures\\UI\\Copyrights\\Copyright_03.bmp"),D3DCOLOR_XRGB(0,0,0)); //Create Copyright02 Texture

    //Create Timer Class
	gr_Timer = new CGRDeltaTimer(); //Construct A CGRDeltaTimer Class

	//Create Camera Class
	gr_Camera = new CGRCamera(m_pDevice,FormL_HWnd); //Construct A CGRCamera Class
	gr_Camera->SetCameraPos(5.0f,50.78f,5.0f); //Set Player Camera Position In Space
	gr_Camera->SetCameraSpeed(40.0f); //Set Player Camera Speed
    gr_Camera->SetMouseSpeed(0.3f); //Set Mouse Speed
	gr_Camera->SetJumpSpeed(70.0f); //Set Jump Speed

	//Create Physics Class
    gr_PhysX = new CGRPhysics(m_pDevice); //Construct A CGRPhysics Class

    //Create SkyBox Class
	gr_SkyBox = new CGRSkyBox(m_pDevice); //Construct A CGRSkyBox Class  
    gr_SkyBox->CreateBox(100.0f,100.0f,100.0f); //Create Our SkyBox Model With Size WHD
    gr_SkyBox->SetTextures(TEXT("\Textures\\SkyBox\\BrightDay\\zneg.png"),
	                       TEXT("\Textures\\SkyBox\\BrightDay\\xneg.png"),
						   TEXT("\Textures\\SkyBox\\BrightDay\\zpos.png"),
						   TEXT("\Textures\\SkyBox\\BrightDay\\xpos.png"),
						   TEXT("\Textures\\SkyBox\\BrightDay\\yneg.png"),
						   TEXT("\Textures\\SkyBox\\BrightDay\\ypos.png"));
    
	//Creation And Loading Simple Map Model Class
    gr_Map = new CGRMap();
    gr_Map->SetTexturePath("\Models\\Locations\\Room_01\\");
    gr_Map->Create(m_pDevice,L"\Models\\Locations\\Room_01\\Test_Room.x");
	//L"\Models\\Locations\\Room_01\\Test_Room.x"
	//L"\Models\\Maps\\Terrain\\Terrain.x"
    //L"\Models\\StarWars\\Chu_Untor\\Chu_Untor.x"

    //Generate Triangles Adjacency For Physics Collision
	gr_PhysX->TrianglesFromDXMesh(gr_Map->GetLocalMeshGeometry()); 
	gr_PhysX->Init(GRPHYSX_ROOM);



	//Create Simple Weapon Class
    gr_Weapon = new CGRWeapon(m_pDevice); 
    
    //Creation And Loading Architecture Model Class
    //gr_Arch_01 = new CGRModels();
    //gr_Arch_01->SetTexturePath("\Textures\\StarWars\\Chu_Untor\\");
    //gr_Arch_01->CreateModelFromX(m_pDevice,L"\Models\\StarWars\\Chu_Untor\\Chu_Untor.x");

	//Creation Mp3 Music Class
    /* gr_Music = new CGRMusic(); //Construct A CGMusic Class  
    gr_Music->CreateSound(L"\Music\\Mp3\\Moby - Live Forever.mp3");
    if (gr_Music != NULL) gr_Music->PlaySound(); */

    gr_SoundsEff = new CGRSoundEffects(); //Construct Sound Effects Container Class

    //Create Gandon Caption Label 
    Vitia_Caption = new CGRLabel(m_pDevice); //Construct A Gandon Caption Label
    Vitia_Caption->FontColor = clWhite; //Set Font Color
    Vitia_Caption->FontName = "MS Sans Serif"; //Set Font Name
    Vitia_Caption->FontSize = 10; //Set Font Size
    Vitia_Caption->FontStyle = FS_BOLD; //Set Font Style
    Vitia_Caption->Left = 14;
    Vitia_Caption->Top = 31;
    Vitia_Caption->Update(); //Update Label Parameters

    //Create Debug Caption Label 
    Debug_Caption = new CGRLabel(m_pDevice); //Construct A Debug Caption Label
    Debug_Caption->FontColor = clWhite; //Set Font Color
    Debug_Caption->FontName = "MS Sans Serif"; //Set Font Name
    Debug_Caption->FontSize = 10; //Set Font Size
    Debug_Caption->FontStyle = FS_BOLD; //Set Font Style
    Debug_Caption->Left = 738;
    Debug_Caption->Top = 31;
    Debug_Caption->Update(); //Update Label Parameters

    //Create Gandon Phrase-1 Label 
    Vitia_Phr1= new CGRLabel(m_pDevice); //Construct A Gandon Phrase-1 Label
    Vitia_Phr1->FontColor = clWhite; //Set Font Color
    Vitia_Phr1->FontName = "MS Sans Serif"; //Set Font Name
    Vitia_Phr1->FontSize = 10; //Set Font Size
    Vitia_Phr1->FontStyle = FS_BOLD; //Set Font Style
    Vitia_Phr1->Left = 126;
    Vitia_Phr1->Top = 60;
    Vitia_Phr1->Update(); //Update Label Parameters

	//Create Gandon Phrase-2 Label 
    Vitia_Phr2 = new CGRLabel(m_pDevice); //Construct A Gandon Phrase-2 Label
    Vitia_Phr2->FontColor = clWhite; //Set Font Color
    Vitia_Phr2->FontName = "MS Sans Serif"; //Set Font Name
    Vitia_Phr2->FontSize = 10; //Set Font Size
    Vitia_Phr2->FontStyle = FS_BOLD; //Set Font Style
    Vitia_Phr2->Left = 126;
    Vitia_Phr2->Top = 74;
    Vitia_Phr2->Update(); //Update Label Parameters

  }

  if (m_pDirect3D != NULL) m_pDirect3D->Release(); //No Longer Needed, Release It
  return m_pResult; //Finally Return Resulting Value
} //EndCreateDirect3DFunction

//-----------------------------------------------------------------------------
// Name: RenderingDirect3D()
// Desc: Draw Direct3D Scene
//-----------------------------------------------------------------------------
void RenderingDirect3D()
{   
  if (m_pDevice != NULL) 
  {
	if (gr_Timer != NULL) gr_Timer->UpdateTimer(); //Update GreenRay Timer Every Frame
	if (gr_Camera != NULL) gr_Camera->UpdateCamera(); //Update GreenRay Camera 
	
    if (gr_PhysX != NULL) 
	{
	  gr_PhysX->UpdatePhysXPlayer(); //Update PhysX Player
	  gr_PhysX->BeginUpdatePhysX(); //Update PhysX
	}

/*    //When Song Is Ending Then Load And Playback Music Again
	if (gr_Music != NULL && gr_Music->IsUnplaying() == TRUE)
	{ 
      Inc_Song++;
      gr_Music->bUnplay = FALSE;
	  gr_Music->Release(); 

	  if (Inc_Song == 1) gr_Music->CreateSound(L"\Music\\Mp3\\Hitman Theme_02.mp3");
	  if (Inc_Song == 2) gr_Music->CreateSound(L"\Music\\Mp3\\StillSuckingNature.mp3");
	
	  if (Inc_Song < 3) gr_Music->PlaySound();
	}*/

    //Clear Back Buffer And Enable Z-Buffer
    m_pDevice->Clear(0,NULL,D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER/*|D3DCLEAR_STENCIL*/,D3DCOLOR_XRGB(20,60,150),1.0f,0);
    
	//Begin Scene
    if (SUCCEEDED(m_pDevice->BeginScene()))
    {
      //Here Code For SceneInit 
	  if (gr_SkyBox != NULL) gr_SkyBox->Render(); //Render The SkyBox ! 

	  RenderHardwareTnL(m_pDevice,D3DXVECTOR3(-1.0f, -1.0f, 0.0f),0); //Render First Harware T&L
      //RenderHardwareTnL(m_pDevice,D3DXVECTOR3(1.0f, 0.0f, 1.0f),1); //Render Second Harware T&L
	
	  //gr_TestModel->RotationModel(0.0f, 0.0f, 0.0f);
      //gr_TestModel->ScaleModel(0.019f, 0.019f, 0.019f);
      //gr_TestModel->DispositionModel(1.0f, 1.0f, 1.0f);


	  if (gr_Map != NULL) gr_Map->Render(); //Render Simple Map Model
 
      gr_PhysX->RampBullet();


      

	  //Render Weapon With HUD
	  gr_HUD01->RenderUIOverlay(); //Rendering The HUD_01
	  if (gr_Weapon != NULL) gr_Weapon->Render(); //Render Weapon 	

      //Render Gandon Console
	  gr_HUD02->RenderUIOverlay(); //Rendering The HUD_02 	
      
	  //Render Debug Console
	  gr_HUD03->RenderUIOverlay(); //Rendering The HUD_03 	

	  //Set Gandon Caption
	  Vitia_Caption->Caption("Консоль для г:");

      //Set Debug Caption
	  Debug_Caption->Caption("Debug window:");

      //Render Debug Console
	  Vitia_Photo->RenderUIOverlay(); //Rendering The Vitia_Photo 

      //Render Debug Console
	  Vitia_Say->RenderUIOverlay(); //Rendering The Gangon Say Panel 




      //Set Gandon Phrase-1
	  Vitia_Phr1->Caption("!!!");

	  //Set Gandon Phrase-2
	  Vitia_Phr2->Caption("");

       
    
    

	  gr_FPSUI->RenderUIOverlay(); //Rendering The User Interface
      if (gr_FPS != NULL) gr_FPS->Render(); //Output Current FPS
	  
      gr_Crosshair->RenderUIOverlay(); //Rendering The Crosshair
	  //gr_Console->RenderUIOverlay(); //Rendering The Console
	  gr_Copyright02->RenderUIOverlay(); //Rendering The Copyright02


	  m_pDevice->EndScene();
    }
    m_pDevice->Present(NULL,NULL,NULL,NULL);
	if (gr_PhysX != NULL) gr_PhysX->EndUpdatePhysX();
  }
} //EndRenderingDirect3DProcedure

//-----------------------------------------------------------------------------
// Name: ReleaseDirect3D()
// Desc: Free Resources And Destroy Direct3D Device
//-----------------------------------------------------------------------------
void ReleaseDirect3D()
{
  //Free UI Classes
  if (gr_FPS != NULL) { gr_FPS->Release(); ReleaseClass(gr_FPS); DeleteClass(gr_FPS); } //Free FPS Counter
  if (gr_FPSUI != NULL) { gr_FPSUI->Release(); ReleaseClass(gr_FPSUI); DeleteClass(gr_FPSUI); } //Free FPSUI Class
  if (gr_Crosshair != NULL) { gr_Crosshair->Release(); ReleaseClass(gr_Crosshair); DeleteClass(gr_Crosshair); } //Free Crosshair Class
  if (gr_Console != NULL) { gr_Console->Release(); ReleaseClass(gr_Console); DeleteClass(gr_Console); } //Free Console Class
  if (gr_HUD01 != NULL) { gr_HUD01->Release(); ReleaseClass(gr_HUD01); DeleteClass(gr_HUD01); } //Free HUD_01 Class
  if (gr_HUD02 != NULL) { gr_HUD02->Release(); ReleaseClass(gr_HUD02); DeleteClass(gr_HUD02); } //Free HUD_02 Class
  if (gr_HUD03 != NULL) { gr_HUD03->Release(); ReleaseClass(gr_HUD03); DeleteClass(gr_HUD03); } //Free HUD_03 Class
  if (Vitia_Photo != NULL) { Vitia_Photo->Release(); ReleaseClass(Vitia_Photo); DeleteClass(Vitia_Photo); } //Free Vitia_Photo Class
  if (Vitia_Say != NULL) { Vitia_Say->Release(); ReleaseClass(Vitia_Say); DeleteClass(Vitia_Say); } //Free Vitia_Say Panel Class
  if (gr_Copyright02 != NULL) { gr_Copyright02->Release(); ReleaseClass(gr_Copyright02); DeleteClass(gr_Copyright02); } //Free Copyright02 Class
  
  //Free Map Model Classes
  if (gr_Map != NULL) { gr_Map->Release(); ReleaseClass(gr_Map); DeleteClass(gr_Map); } //Free Map Model Class
 
  if (gr_SkyBox != NULL) { gr_SkyBox->Release(); ReleaseClass(gr_SkyBox); DeleteClass(gr_SkyBox); } //Free SkyBox Class
  if (gr_Camera != NULL) { /*gr_Camera->Release();*/ ReleaseClass(gr_Camera); DeleteClass(gr_Camera); } //Free Camera Class  
  if (gr_PhysX != NULL) { gr_PhysX->Release(); /*ReleaseClass(gr_PhysX); DeleteClass(gr_PhysX);*/ } //Free Physics Class
  if (gr_Music != NULL) { gr_Music->StopSound(); gr_Music->Release(); ReleaseClass(gr_Music); DeleteClass(gr_Music); } //Free Mp3 Music Class
  if (gr_SoundsEff != NULL) { gr_SoundsEff->Release(); ReleaseClass(gr_SoundsEff); DeleteClass(gr_SoundsEff); } //Free Sounds Effects Class

  if (m_pDevice != NULL) m_pDevice->Release(); //Release The Direct3D Device
} //EndReleaseDirect3DProcedure

//------------------------------------------------------------------------------