/******************************************************************************
// It's Unofficial Version Of The GreenRay Engine v.3.0.0.2
// GR_Methods.h
// It's The GreenRay Direct3D Useful Control Interfaces Engine Header File.
// Made Specially For Implementing Useful Engine System Manipulations Parameters.
// Date Creation: 29 August 2005
// Last Changing: 03 July 2009
// Author Rights By: Zie Technology, Verg Soft And Lucky's Flash Studio Inc.,
// Licensed By: (C)DDD Digital Dreams Development And (C)3D Studios Inc., 2009
******************************************************************************/
#include <GR_Engine.h>

////////////////////////////////////////////////////////////////////////////////
// Directives For Release Classes
////////////////////////////////////////////////////////////////////////////////
#define ReleaseClass(_Ptr) { if (_Ptr) { (_Ptr)->Release(); (_Ptr) = NULL; } }
//Directive For Delete Classes
#define DeleteClass(_Ptr) { if (_Ptr) { delete(_Ptr); (_Ptr) = NULL; } }
//Directive For Delete Arrays
#define DeleteArray(_Ptr) { if (_Ptr) { delete[](_Ptr); (_Ptr) = NULL; } }

////////////////////////////////////////////////////////////////////////////////
// Exporting Procedures And Functions
////////////////////////////////////////////////////////////////////////////////
void ShowMessage(LPCSTR Mes_String);

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