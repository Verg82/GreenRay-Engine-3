/******************************************************************************
// It's Unofficial Version Of The GreenRay Engine v.3.0.0.1
// GRMethods.h
// It's The GreenRay Direct3D Useful Control Interfaces Engine Header File.
// Made Specially For Implementing Useful Engine System Manipulations Parameters.
// Date Creation: 29 August 2005
// Last Changing: 16 Augist 2006
// Author Rights By: Zie Technology, Zombie Soft And Lucky's Flash Studio Inc.,
// Licensed By: (C)DDD Digital Dreams Development And (C)3D Studios Inc., 2006
******************************************************************************/
#include <GREngine.h>
#include <GR_Label.h>

////////////////////////////////////////////////////////////////////////////////
// Directives For Release Classes
////////////////////////////////////////////////////////////////////////////////
#define ReleaseClass(_Ptr) { if (_Ptr) { (_Ptr)->Release(); (_Ptr) = NULL; } }
//Directive For Delete Classes
#define DeleteClass(_Ptr) { if (_Ptr) { delete(_Ptr); (_Ptr) = NULL; } }
//Directive For Delete Arrays
#define DeleteArray(_Ptr) { if (_Ptr) { delete[](_Ptr); (_Ptr) = NULL; } }

////////////////////////////////////////////////////////////////////////////////
// Directives Or Helper Functions Or Procedures
////////////////////////////////////////////////////////////////////////////////
DWORD FloatToDWord(FLOAT F);

////////////////////////////////////////////////////////////////////////////////
// Exporting Procedures And Functions
////////////////////////////////////////////////////////////////////////////////
void ShowMessage(LPCSTR _MessageString); //Rebuild From Delphi Syntax On C++
void InitMaterial(D3DMATERIAL9& _Mtrl,float r,float g,float b,float a); //Setting Up Matherial Directives

////////////////////////////////////////////////////////////////////////////////
// Classes Used By This Header
////////////////////////////////////////////////////////////////////////////////
//class CGRFPSCounter;
//class CGRDeltaTimer;
//class CGRTimer;

//-----------------------------------------------------------------------------
// Name: Struct CGRDeltaTimer
// Desc:
//-----------------------------------------------------------------------------
#if !defined(AFX_DELTATIMER_H__)
#define AFX_DELTATIMER_H__

class CGRDeltaTimer
{ 
public:
  /*Public Directives*/
  CGRDeltaTimer();
  virtual ~CGRDeltaTimer();
  void UpdateTimer();
  float GetTime();
private:
  /*Private Directives*/
  DWORD CurrentTime; //Current Timer Value
  DWORD LastTime;    //Previous Timer Value
  float DeltaTime;   //Time Elapsed Since Last Frame
};
#endif //!defined(AFX_DELTATIMER_H__)

//-----------------------------------------------------------------------------
// Name: Struct CGRTimer
// Desc:
//-----------------------------------------------------------------------------
#if !defined(AFX_TIMER_H__)
#define AFX_TIMER_H__

class CGRTimer
{ 
public:
  /*Public Directives*/  
  CGRTimer();
  virtual ~CGRTimer();
  
  void Reset(); //Resets The Timer
  void Start(); //Starts The Timer
  void Stop(); //Stop (Or Pause) The Timer
  void Advance(); //Advance The Timer By 0.1 Seconds
  double GetAbsoluteTime(); //Get The Absolute System Time
  double GetTime(); //Get The Current Time
  double GetElapsedTime(); //Get The Time That Elapsed Between GetElapsedTime() Calls
  bool IsStopped(); //Returns True If Timer Stopped
private:
  /*Private Directives*/
protected:
  /*Protected Directives*/
  bool m_bUsingQPF;
  bool m_bTimerStopped;
  LONGLONG m_llQPFTicksPerSec;

  LONGLONG m_llStopTime;
  LONGLONG m_llLastElapsedTime;
  LONGLONG m_llBaseTime;
};
#endif //!defined(AFX_TIMER_H__)

////////////////////////////////////////////////////////////////////////////////
// Global Variables Or Constants
////////////////////////////////////////////////////////////////////////////////
extern CGRDeltaTimer* gr_Timer; //GreenRay Timer Interface Class

//-----------------------------------------------------------------------------
// Name: Struct CGRFPSCounter
// Desc:
//-----------------------------------------------------------------------------
#if !defined(AFX_FPSCOUNTER_H__)
#define AFX_FPSCOUNTER_H__

class CGRFPSCounter
{ 
public:
  /*Public Directives*/
  
  CGRFPSCounter(LPDIRECT3DDEVICE9 pDevice);
  virtual ~CGRFPSCounter();
  void Release();
  void Render();

private:
  /*Private Directives*/
  LPDIRECT3DDEVICE9 d_pDevice; //Pointer On Direct3D Device Interface
  CGRLabel* m_pFPS; //General GreenRay FPS Font Interface
  float m_fFPS; //Instanteous Frame Rate
  TCHAR m_strDeviceStats[64]; //String To Hold D3D Device Stats
  TCHAR pFrameStats[64]; //String To Hold Frame Stats
  CGRTimer* m_pTimer; //GreenRay Timer Interface

  void RefreshFPS(); //General FPS Counter Procedure
};
#endif //!defined(AFX_FPSCOUNTER_H__)