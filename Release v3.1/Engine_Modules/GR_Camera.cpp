/*******************************************************************************
// It's Unofficial Version Of The GreenRay Engine v.3.0.0.2
// GR_Camera.cpp
// It's The GreenRay Direct3D Camera Control Interfaces Engine.
// Made Specially For Implementing Camera System Manipulations Parameters.
// Date Creation: 29 August 2005
// Last Changing: 20 January 2007
// Licensed By: (C)DDD Digital Dreams Development And (C)3D Studios Inc., 2007
*******************************************************************************/
#include <GR_Camera.h>

//////////////////////////////////////////////////////////////////////////////
// Construction/Destruction For CGRCamera Class
//////////////////////////////////////////////////////////////////////////////
CGRCamera::CGRCamera(LPDIRECT3DDEVICE9 pDevice,HWND _HWnd)
{
  d_pDevice = pDevice; //Apropritate Device Pointer To Class
  m_pDirectInput = NULL;
  m_pMouse = NULL;
  m_pKeyboard = NULL;
 
  //Create Input Devices
  if (SUCCEEDED(CreateInputDevices(_HWnd)))
  {
    m_pMouse->Acquire();
	m_pKeyboard->Acquire();
  }
  
  m_fCameraSpeed = 0.01f;
  m_fMouseSpeed = 0.01f;
  m_fJumpSpeed = 0.01f;
  Pitch = 0.0f;
  Yaw = 0.0f;
  Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
  Target = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
  Up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
  Velocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
  D3DXVec3Normalize(&Look_Vector,&(Target-Position));

  //Set The Projection Matrix
  float Aspect_Ratio = 1.0f;
  D3DXMatrixPerspectiveFovLH(&Projection,D3DX_PI/4,Aspect_Ratio,0.1f,1000.0f);
  d_pDevice->SetTransform(D3DTS_PROJECTION,&Projection);

  m_pTimer = new CGRTimer(); //Construct A Timer Class
} //EndConstructionDirectives

CGRCamera::~CGRCamera()
{
} //EndDestructionDirectives

//-----------------------------------------------------------------------------
// Name: CGRCamera::Release()
// Desc: 
//-----------------------------------------------------------------------------
void CGRCamera::Release()
{
  m_pMouse->Unacquire(); //UnControl Mouse
  m_pKeyboard->Unacquire(); //UnControl Keyboard

  ReleaseClass(m_pMouse); //Release Mouse
  ReleaseClass(m_pKeyboard); //Release Keyboard

  ReleaseClass(m_pDirectInput); //Release DirectInput
} //EndReleaseProcedure

//-----------------------------------------------------------------------------
// Name: CGRCamera::CreateInputDevices()
// Desc: 
//-----------------------------------------------------------------------------
HRESULT CGRCamera::CreateInputDevices(HWND _HWnd)
{
  HRESULT Result = S_OK;
  //Create InputDevice
  if (FAILED(DirectInput8Create(GetModuleHandle(NULL),DIRECTINPUT_VERSION,
  IID_IDirectInput8,(void**)&m_pDirectInput,NULL))) 
  {
    MessageBoxA(NULL,"Can't Create DirectInputContext","GreenRay Engine Error",
	MB_SYSTEMMODAL|MB_ICONEXCLAMATION); 
	Result = E_FAIL;
  }

  //Create Mouse Input Device
  if (FAILED(m_pDirectInput->CreateDevice(GUID_SysMouse,&m_pMouse,NULL)))
  {
    MessageBoxA(NULL,"Can't Create MouseInputDevice","GreenRay Engine Error",
	MB_SYSTEMMODAL|MB_ICONEXCLAMATION); 
	m_pDirectInput->Release(); 
	Result = E_FAIL;
  }

  if (FAILED(m_pMouse->SetDataFormat(&c_dfDIMouse2))) 
  {
    MessageBoxA(NULL,"Can't Set MouseDataFormat","GreenRay Engine Error",
	MB_SYSTEMMODAL|MB_ICONEXCLAMATION);
    m_pMouse->Release();
    m_pDirectInput->Release();
    m_pMouse = NULL;
    m_pDirectInput = NULL;
	Result = E_FAIL;
  }

  if (FAILED(m_pMouse->SetCooperativeLevel(_HWnd,DISCL_FOREGROUND|DISCL_EXCLUSIVE)))
  {
    MessageBoxA(NULL,"Can't Set Mouse CooperativeLevel","GreenRay Engine Error",
	MB_SYSTEMMODAL|MB_ICONEXCLAMATION);
    m_pMouse->Release();
	m_pDirectInput->Release();
	m_pMouse = NULL;
    m_pDirectInput = NULL;
	Result = E_FAIL;
  }

  //Create Keyboard Input Device
  if FAILED(m_pDirectInput->CreateDevice(GUID_SysKeyboard,&m_pKeyboard,NULL))
  {
    MessageBoxA(NULL,"Can't Create KeyboardInputDevice","GreenRay Engine Error",
	MB_SYSTEMMODAL|MB_ICONEXCLAMATION);
    m_pKeyboard->Release();
	m_pKeyboard = NULL;
	Result = E_FAIL;
  }
        
  if (FAILED(m_pKeyboard->SetDataFormat(&c_dfDIKeyboard))) 
  {
    MessageBoxA(NULL,"Can't Set KeyboardDataFormat","GreenRay Engine Error",
	MB_SYSTEMMODAL|MB_ICONEXCLAMATION);
    m_pMouse->Release();
	m_pKeyboard->Release();
	m_pDirectInput->Release();
    m_pDirectInput = NULL;
    Result = E_FAIL;
  }

  if (FAILED(m_pKeyboard->SetCooperativeLevel(_HWnd,DISCL_FOREGROUND|DISCL_NONEXCLUSIVE)))
  {
    MessageBoxA(NULL,"Can't Set Keyboard CooperativeLevel","GreenRay Engine Error",
	MB_SYSTEMMODAL|MB_ICONEXCLAMATION);
    m_pMouse->Release();
	m_pKeyboard->Release();
	m_pDirectInput->Release();
    m_pDirectInput = NULL;
	Result = E_FAIL;
  }

  /*DIPROPDWORD dipdw = {
	{
    sizeof(DIPROPDWORD),  //diph.dwSize
	sizeof(DIPROPHEADER), //diph.dwHeaderSize
	0,                    //diph.dwObj
	DIPH_DEVICE,},        //diph.dwHow
	DINPUT_BUFFERSIZE256, //dwData
  };
           
  if (FAILED(m_pKeyboard->SetProperty(DIPROP_BUFFERSIZE,&dipdw.diph)))
  {
    MessageBoxA(NULL,"Can't Set Property For KeyboardDevice","GreenRay Engine Error",
	MB_SYSTEMMODAL|MB_ICONEXCLAMATION);
    m_pMouse->Release();
	m_pKeyboard->Release();
	m_pDirectInput->Release();
    m_pDirectInput = NULL;
	return;
  } */

  return Result;
} //EndCreateInputDevicesFunction

//-----------------------------------------------------------------------------
// Name: CGRCamera::UpdateCamera()
// Desc: Poll The DirectInput Devices
//-----------------------------------------------------------------------------
void CGRCamera::UpdateCamera()
{ 
  DIMOUSESTATE2 Mouse_Data; //Value Keep Mouse States
  char Key_Buffer[256]; //Buffer To Hold Keyboard States

  //d_pDevice->TestCooperativeLevel(); 
  float m_fForward = 0;
  float m_fSideward = 0;

  //Donation: GetMouseState 
  if (m_pMouse != NULL) //Check On Ready Mouse
  {         
    //Get Mouse Input Data
    ZeroMemory(&Mouse_Data,sizeof(Mouse_Data));
    m_pMouse->GetDeviceState(sizeof(Mouse_Data),&Mouse_Data);
    m_pMouse->Acquire();

    //Here Getting Mouse States:
	Pitch-= (double)Mouse_Data.lY*gr_Timer->GetTime()*m_fMouseSpeed;
	Yaw-= (double)Mouse_Data.lX*gr_Timer->GetTime()*m_fMouseSpeed;
   
	//Mouse Button States (0- It's Button Up)
    if ((Mouse_Data.rgbButtons[0x00] & 0x80) != 0) 
	{
	  //Update Weapon Shoot By Timer For Automatic Mode:
	  /*static float fLastTime = 0.0f; //Timing Variality Directives
	  double fTime = m_pTimer->GetAbsoluteTime();

	  //Update The Scene Stats Once Per Second
      if (fTime-fLastTime > 0.097f)
      {
        fLastTime = fTime;
        gr_Weapon->WeaponShoot(); 
      }*/

	  //Here Update Weapon For Single Shoot Mode:
	  if (!ShoottiDown) gr_Weapon->WeaponShoot(); 
	  ShoottiDown = true;
	}
	if ((Mouse_Data.rgbButtons[0x00] & 0x80) == 0) { ShoottiDown = false; }  
    
	//if (Mouse_Data.rgbButtons[0x01] & 0x80) PlaybackSound("KudaTam"); 
    //if (Mouse_Data.rgbButtons[0x02] & 0x80) PlaybackSound("ZachemTam"); */
  }

  //Donation: GetKeyboardState
  if (m_pKeyboard != NULL) //Check On Ready Keyboard
  {
    //Keyboard Input Data
    ZeroMemory(&Key_Buffer,sizeof(Key_Buffer));
    m_pKeyboard->GetDeviceState(sizeof(Key_Buffer),Key_Buffer);
    m_pKeyboard->Acquire();

    //Keyboard Contol Input Mode
    if (Key_Buffer[DIK_W] & 0x80) m_fForward = m_fCameraSpeed*gr_Timer->GetTime();
    if (Key_Buffer[DIK_S] & 0x80) m_fForward = -m_fCameraSpeed*gr_Timer->GetTime();
    if (Key_Buffer[DIK_A] & 0x80) m_fSideward = -m_fCameraSpeed*gr_Timer->GetTime();
    if (Key_Buffer[DIK_D] & 0x80) m_fSideward = m_fCameraSpeed*gr_Timer->GetTime();
	if (Key_Buffer[DIK_SPACE] & 0x80) gr_PhysX->JumpPhysXPlayer(m_fJumpSpeed); //Make The Player Jump

	//BOOL m_bShiftStatus;
    //if (Key_Buffer[DIK_LSHIFT] & 0x80) m_bShiftStatus = TRUE;
    
	//(~) Set Enable\Disable Console Key
    //if (Key_Buffer[0x29] & 0x80) PlaybackSound("GdeKonsol");
  }

  //Restrict The Ability To Look Too High Or Too Low
  if (Pitch < -1.56f) Pitch = -1.56f; //Y
  if (Pitch > 1.56f) Pitch = 1.56f;
  
  if (Yaw >= 6.28f) Yaw = 0.0f;  //X
  if (Yaw <= -6.28f) Yaw = 0.0f;

  //Get Camera X,Y,Z Position Coordinates
  PosX = (cosf(Pitch)*cosf(Yaw)*10.0f);
  PosY = (sinf(Pitch)*10.0f);
  PosZ = (sinf(Yaw)*cosf(Pitch)*10.0f);

  //Set The Target Of The Camera
  Target = D3DXVECTOR3(PosX,PosY,PosZ)+Position;

  //Update The Look Vector
  D3DXVec3Normalize(&Look_Vector,&(Target-Position));
  D3DXVECTOR3 XZLook_Vector = Look_Vector;
  XZLook_Vector.y = 0;
  D3DXVec3Normalize(&XZLook_Vector,&XZLook_Vector);
  D3DXVECTOR3 Side_Vector(XZLook_Vector.z,0.0f,-XZLook_Vector.x);
  Velocity = (XZLook_Vector*m_fForward)+(Side_Vector*m_fSideward);
  
  /*Position+= (XZLook_Vector*m_fForward)+(Side_Vector*m_fSideward);
  Target+= (XZLook_Vector*m_fForward)+(Side_Vector*m_fSideward);

  //Update The View Matix
  D3DXMatrixLookAtLH(&View,&Position,&Target,&Up);
  //Update The Active View
  d_pDevice->SetTransform(D3DTS_VIEW,&View);*/
} //EndUpdateCameraProcedure

//-----------------------------------------------------------------------------
// Name: CGRCamera::SetCameraPos()
// Desc: 
//-----------------------------------------------------------------------------
void CGRCamera::SetCameraPos(float Pos_X,float Pos_Y,float Pos_Z)
{
  Position = D3DXVECTOR3(Pos_X,Pos_Y,Pos_Z);
  //Target = D3DXVECTOR3(1.6f, -0.018f, 0.0f);
} //EndSetCameraPosProcedure

//-----------------------------------------------------------------------------
// Name: CGRCamera::SetCameraSpeed()
// Desc: 
//-----------------------------------------------------------------------------
void CGRCamera::SetCameraSpeed(float Cam_Speed)
{
  m_fCameraSpeed = Cam_Speed;
} //EndSetCameraSpeedProcedure

//-----------------------------------------------------------------------------
// Name: CGRCamera::SetMouseSpeed()
// Desc: 
//-----------------------------------------------------------------------------
void CGRCamera::SetMouseSpeed(float Mouse_Speed)
{
  m_fMouseSpeed = Mouse_Speed;
} //EndSetMouseSpeedProcedure

//-----------------------------------------------------------------------------
// Name: CGRCamera::SetJumpSpeed()
// Desc: 
//-----------------------------------------------------------------------------
void CGRCamera::SetJumpSpeed(float Jump_Speed)
{
  m_fJumpSpeed = Jump_Speed;
} //EndSetJumpSpeedProcedure

//-----------------------------------------------------------------------------