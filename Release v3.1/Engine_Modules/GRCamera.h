/*******************************************************************************
// It's Unofficial Version Of The GreenRay Engine v.3.0.0.1
// GRCamera.h: Interface For The TGRCamera Class.
// It's The GreenRay Direct3D Camera Control Interfaces Engine.
// Made Specially For Implementing Camera System Manipulations Parameters.
// Date Creation: 29 August 2005
// Last Changing: 23 March 2006
// Author Rights By: Zie Technology, Zombie Soft And Lucky's Flash Studio Inc.,
// Licensed By: (C)DDD Digital Dreams Development And (C)3D Studios Inc., 2006
*******************************************************************************/
#include <GREngine.h>

void OutputCameraPositionXY();

////////////////////////////////////////////////////////////////////////////////
// Global Variables Or Constants
////////////////////////////////////////////////////////////////////////////////
const int gr_CameraUp = 0x000001;
const int gr_CameraDown = 0x000002;
const int gr_CameraLeft = 0x000003;
const int gr_CameraRight = 0x000004;
const int gr_CameraJump = 0x000005;

////////////////////////////////////////////////////////////////////////////////
// Classes Used By This Header
////////////////////////////////////////////////////////////////////////////////
//class CGRCamera;

//-----------------------------------------------------------------------------
// Name: Struct CGRCamera
// Desc: 
//-----------------------------------------------------------------------------
#if !defined(AFX_CAMERA_H__)
#define AFX_CAMERA_H__
class CGRCamera
{	
public:
  /*Public Directives*/
  CGRCamera(LPDIRECT3DDEVICE9 pDevice,HWND _HWnd,bool bConnect,bool bNoClip);
  virtual ~CGRCamera();
  void Release();
  
  void ResetCamera();
  void CameraMoveSpeed(float MoveSpeed);
  void SetCameraPerspective();
  void UpdateCamera();
  void UpdateDeltaTimer();
  void UpdateVelocity();
  
  float GetCameraX() { return _XLocate; } //Return Camera XLocation Position
  float GetCameraY() { return _YLocate; } //Return Camera YLocation Position
  float GetCameraZ() { return _ZLocate; } //Return Camera ZLocation Position
  D3DXMATRIX CameraWorld() { return _MatrixWorld; } //Return Camera World Position
  D3DXMATRIX CameraView() { return _MatrixView; } //Return Camera View Position
  D3DXMATRIX CameraProjection() { return _MatrixProjection; } //Return Camera Projection Position
private:
  /*Private Directives*/ 
  LPDIRECT3DDEVICE9 d_pDevice;      //Pointer On Direct3D Device Interface
  IDirectInput8* m_pDirectInput;    //Base DirectInput Device
  IDirectInputDevice8* m_pMouse;    //Mouse Input Device
  IDirectInputDevice8* m_pKeyboard; //Keyboard Input Device

  float m_fCameraSpeed; //Camera Speed Factor
  bool m_bConnected;    //Connect State To Camera
  bool m_bNoClip;       //NoClip Mode
  bool m_bVelocity;     //Velocity State
  bool m_bUp;           //Jump Up State

  D3DXVECTOR3 m_fEyePoint,m_fLookAtPoint,m_fUpVecPoint; //Location Matrices
  D3DXMATRIX _MatrixWorld,_MatrixView,_MatrixProjection; //Camera Matrices
  float _XLocate,_YLocate,_ZLocate;
  float _VecX,_VecY,_VecZ,_TmpVecY;

  //Timing Variables
  DWORD CurrentTime; //Current Timer Value
  DWORD LastTime;    //Previous Timer Value
  float DeltaTime;   //Time Elapsed Since Last Frame

  void CreateInputDevices(HWND _HWnd);
  void GetMouseState();
  void GetKeyboardState();	
  void MoveCamera(int _Direction);
};
#endif //!defined(AFX_CAMERA_H__)

