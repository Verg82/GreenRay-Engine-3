/****************************************************************************/ 
// It's Unofficial Version Of The GreenRay Engine v.3.0.0.2
// GR_Camera.h
// It's An GreenRay GR_Camera Header File.
// Date Creation: 29 August 2005
// Last Changing: 09 February 2009
// Licensed By: (C)DDD Digital Dreams Development And (C)3D Studios Inc., 2009
/*****************************************************************************/
#pragma once
#include <GR_Engine.h>
#include <GR_Methods.h>

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
  CGRCamera(LPDIRECT3DDEVICE9 pDevice);
  virtual ~CGRCamera();
  void Release();
  
  HRESULT CreateInputDevices(HWND _HWnd);
  void UpdateCamera();
  
  //��������� ���������� ������������ ������ � ������� ���� ���
  //������ ��������� "����" � ������ ��������������.
  void SetCameraPos(D3DXVECTOR3 Pos_XYZ) { Position = Pos_XYZ; }

  //�������� ������������ ������, �.� ���������� �����
  //�� ����� �������� ��� �������� ������ � ������� ��� ���������� � ����������.
  void SetCameraSpeed(float Cam_Speed) { m_fCameraSpeed = Cam_Speed; } 

  //����� ������� �������� �������� � ������ ����.
  void SetMouseSpeed(float Mouse_Speed) { m_fMouseSpeed = Mouse_Speed; }
 
  //������� ������������ �������������� ������ � ������� ����.
  D3DXVECTOR3 Pos() { return Position; }
private:
  /*Private Directives*/ 
  LPDIRECT3DDEVICE9 d_pDevice;      //Pointer On Direct3D Device Interface
  IDirectInput8* m_pDirectInput;    //Base DirectInput Device
  IDirectInputDevice8* m_pMouse;    //Mouse Input Device
  IDirectInputDevice8* m_pKeyboard; //Keyboard Input Device

  //Base Camera Directives
  float Pitch,Yaw;
  D3DXVECTOR3 Position;
  D3DXVECTOR3 Target;
  D3DXVECTOR3 Up;
  D3DXVECTOR3 Velocity;
  D3DXVECTOR3 Look_Vector; //Base Look Vector
  D3DXMATRIX View;
  D3DXMATRIX Projection;

  float PosX,PosY,PosZ; 

  float m_fCameraSpeed; //Set Camera Speed
  float m_fMouseSpeed; //Set Mouse Speed

  //"�������" ��������� ��� ����������� ����� ��� ��������� 
  //������ ������� ���� �� ��������������� ������ ��� 
  //���������� ���������� ��������������.
  CGRTimer* m_pTimer; //Timing Interface
};
#endif //!defined(AFX_CAMERA_H__)

////////////////////////////////////////////////////////////////////////////////
// Global Variables Or Constants
////////////////////////////////////////////////////////////////////////////////
extern CGRCamera* gr_Camera; //GreenRay Camera Global Interface Class