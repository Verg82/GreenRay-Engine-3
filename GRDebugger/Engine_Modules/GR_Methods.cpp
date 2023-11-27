/******************************************************************************
// It's Unofficial Version Of The GreenRay Engine v.3.0.0.2
// GR_Methods.cpp
// It's The GreenRay Direct3D Useful Control Interfaces Engine Source File.
// Made Specially For Implementing Useful Engine System Manipulations Parameters.
// Date Creation: 29 August 2005
// Last Changing: 03 July 2009
// Author Rights By: Zie Technology, Verg Soft And Lucky's Flash Studio Inc.,
// Licensed By: (C)DDD Digital Dreams Development And (C)3D Studios Inc., 2009
******************************************************************************/
#include <GR_Methods.h>

//-----------------------------------------------------------------------------
// Name: ShowMessage()
// Desc: 
//-----------------------------------------------------------------------------
void ShowMessage(LPCSTR Mes_String)
{
  MessageBoxA(NULL,Mes_String,"GreenRay Engine",MB_SYSTEMMODAL|MB_ICONERROR);
} //EndShowMessageProcedure

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction For CGRDeltaTimer Class
///////////////////////////////////////////////////////////////////////////////
CGRDeltaTimer::CGRDeltaTimer()
{
} //EndConstructionDirectives

CGRDeltaTimer::~CGRDeltaTimer()
{ 
} //EndDestructionDirectives

//-----------------------------------------------------------------------------
// Name: CGRDeltaTimer::UpdateTimer()
// Desc: Execute Each Frame To Establish Time Base
//-----------------------------------------------------------------------------
void CGRDeltaTimer::UpdateTimer()
{
  CurrentTime = timeGetTime();
  DeltaTime = (float)((CurrentTime-LastTime)*0.001f);
  LastTime = CurrentTime;
} //EndUpdateTimerProcedure

//-----------------------------------------------------------------------------
// Name: CGRDeltaTimer::GetTime()
// Desc:
//-----------------------------------------------------------------------------
float CGRDeltaTimer::GetTime()
{
  return DeltaTime;
} //EndGetTimeFunction

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction For CGRTimer Class
///////////////////////////////////////////////////////////////////////////////
CGRTimer::CGRTimer()
{
  m_bUsingQPF         = false;
  m_bTimerStopped     = true;
  m_llQPFTicksPerSec  = 0;

  m_llStopTime        = 0;
  m_llLastElapsedTime = 0;
  m_llBaseTime        = 0;

 //Use QueryPerformanceFrequency() To Get Frequency Of Timer.  
 LARGE_INTEGER qwTicksPerSec;
 m_bUsingQPF = (bool)(QueryPerformanceFrequency(&qwTicksPerSec) != 0);
 m_llQPFTicksPerSec = qwTicksPerSec.QuadPart;
} //EndConstructionDirectives

CGRTimer::~CGRTimer()
{ 
} //EndDestructionDirectives

//-----------------------------------------------------------------------------
// Name: CGRTimer::Reset()
// Desc: 
//-----------------------------------------------------------------------------
void CGRTimer::Reset()
{
  if (!m_bUsingQPF) return;

  //Get Either The Current Time Or The Stop Time
  LARGE_INTEGER qwTime;
  if (m_llStopTime != 0) qwTime.QuadPart = m_llStopTime;
  else QueryPerformanceCounter(&qwTime);

  m_llBaseTime = qwTime.QuadPart;
  m_llLastElapsedTime = qwTime.QuadPart;
  m_llStopTime = 0;
  m_bTimerStopped = FALSE;
} //EndResetProcedure

//-----------------------------------------------------------------------------
// Name: CGRTimer::Start()
// Desc: 
//-----------------------------------------------------------------------------
void CGRTimer::Start()
{
  if (!m_bUsingQPF) return;

  //Get The Current Time
  LARGE_INTEGER qwTime;
  QueryPerformanceCounter(&qwTime);

  if (m_bTimerStopped) m_llBaseTime+= qwTime.QuadPart-m_llStopTime;
  m_llStopTime = 0;
  m_llLastElapsedTime = qwTime.QuadPart;
  m_bTimerStopped = FALSE;
} //EndStartProcedure

//-----------------------------------------------------------------------------
// Name: CGRTimer::Stop()
// Desc: 
//-----------------------------------------------------------------------------
void CGRTimer::Stop()
{
  if (!m_bUsingQPF) return;

  if (!m_bTimerStopped)
  {
    //Get Either The Current Time Or The Stop Time
    LARGE_INTEGER qwTime;
    if (m_llStopTime != 0) qwTime.QuadPart = m_llStopTime;
    else QueryPerformanceCounter(&qwTime);

    m_llStopTime = qwTime.QuadPart;
    m_llLastElapsedTime = qwTime.QuadPart;
    m_bTimerStopped = TRUE;
  }
} //EndStopProcedure

//-----------------------------------------------------------------------------
// Name: CGRTimer::Advance()
// Desc: 
//-----------------------------------------------------------------------------
void CGRTimer::Advance()
{
  if (!m_bUsingQPF) return;
  m_llStopTime+= m_llQPFTicksPerSec/10;
} //EndAdvanceProcedure

//-----------------------------------------------------------------------------
// Name: CGRTimer::GetAbsoluteTime()
// Desc: 
//-----------------------------------------------------------------------------
double CGRTimer::GetAbsoluteTime()
{
  if (!m_bUsingQPF) return -1.0;

  //Get Either The Current Time Or The Stop Time
  LARGE_INTEGER qwTime;
  if (m_llStopTime != 0) qwTime.QuadPart = m_llStopTime;
  else QueryPerformanceCounter(&qwTime);

  double fTime = qwTime.QuadPart/(double) m_llQPFTicksPerSec;
  return fTime;
} //EndGetAbsoluteTimeFunction

//-----------------------------------------------------------------------------
// Name: CGRTimer::GetTime()
// Desc: 
//-----------------------------------------------------------------------------
double CGRTimer::GetTime()
{
  if (!m_bUsingQPF) return -1.0;

  //Get Either The Current Time Or The Stop Time
  LARGE_INTEGER qwTime;
  if (m_llStopTime != 0) qwTime.QuadPart = m_llStopTime;
  else QueryPerformanceCounter(&qwTime);

  double fAppTime = (double)(qwTime.QuadPart-m_llBaseTime)/(double)m_llQPFTicksPerSec;
  return fAppTime;
} //EndGetTimeFunction

//-----------------------------------------------------------------------------
// Name: CGRTimer::GetElapsedTime()
// Desc: 
//-----------------------------------------------------------------------------
double CGRTimer::GetElapsedTime()
{
  if (!m_bUsingQPF) return -1.0;

  //Get Either The Current Time Or The Stop Time
  LARGE_INTEGER qwTime;
  if (m_llStopTime != 0) qwTime.QuadPart = m_llStopTime;
  else QueryPerformanceCounter(&qwTime);

  double fElapsedTime = (double)(qwTime.QuadPart-m_llLastElapsedTime)/(double)m_llQPFTicksPerSec;
  m_llLastElapsedTime = qwTime.QuadPart;

  return fElapsedTime;
} //EndGetElapsedTimeFunction

//-----------------------------------------------------------------------------
// Name: CGRTimer::IsStopped()
// Desc: 
//-----------------------------------------------------------------------------
bool CGRTimer::IsStopped()
{
  return m_bTimerStopped;
} //EndIsStoppedFunction