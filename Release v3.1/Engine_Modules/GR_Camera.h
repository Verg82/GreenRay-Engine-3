/*******************************************************************************
// It's Unofficial Version Of The GreenRay Engine v.3.0.0.2
// GR_Camera.h: Interface For The TGRCamera Class.
// It's The GreenRay Direct3D Camera Control Interfaces Engine.
// Made Specially For Implementing Camera System Manipulations Parameters.
// Date Creation: 29 August 2005
// Last Changing: 20 January 2007
// Licensed By: (C)DDD Digital Dreams Development And (C)3D Studios Inc., 2009
*******************************************************************************/
#pragma once
#include <GREngine.h>
#include <GRMethods.h>
#include <GR_Physics.h>
#include <GR_Weapon.h>
#include <GRSound.h>

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
  CGRCamera(LPDIRECT3DDEVICE9 pDevice,HWND _HWnd);
  virtual ~CGRCamera();
  void Release();
  
  HRESULT CreateInputDevices(HWND _HWnd);
  void UpdateCamera();
  void SetCameraPos(float Pos_X,float Pos_Y,float Pos_Z);
  void SetCameraSpeed(float Cam_Speed);
  void SetMouseSpeed(float Mouse_Speed);
  void SetJumpSpeed(float Jump_Speed);
public:
  /*Public Directives*/
  float Pitch,Yaw;
  float PosX,PosY,PosZ; 
  D3DXVECTOR3 Position;
  D3DXVECTOR3 Target;
  D3DXVECTOR3 Up;
  D3DXVECTOR3 Velocity;
  D3DXVECTOR3 Look_Vector; //General Look Vector
  D3DXMATRIX View;
  D3DXMATRIX Projection;
private:
  /*Private Directives*/ 
  LPDIRECT3DDEVICE9 d_pDevice;      //Pointer On Direct3D Device Interface
  IDirectInput8* m_pDirectInput;    //Base DirectInput Device
  IDirectInputDevice8* m_pMouse;    //Mouse Input Device
  IDirectInputDevice8* m_pKeyboard; //Keyboard Input Device

  float m_fCameraSpeed; //Set Speed Of Camera
  float m_fMouseSpeed; //Set Mouse Speed
  float m_fJumpSpeed; //Set Jump Speed
  
  bool ShoottiDown; //Use For Single Shoot Method
  CGRTimer* m_pTimer; //Timing Interface
};
#endif //!defined(AFX_CAMERA_H__)

////////////////////////////////////////////////////////////////////////////////
// Global Variables Or Constants
////////////////////////////////////////////////////////////////////////////////
extern CGRCamera* gr_Camera; //GreenRay Camera Global Interface Class