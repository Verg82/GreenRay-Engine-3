/******************************************************************************
// It's Unofficial Version Of The GreenRay Engine v.3.0.0.1
// GRMethods.cpp
// It's The GreenRay Direct3D Useful Control Interfaces Engine Source File.
// Made Specially For Implementing Useful Engine System Manipulations Parameters.
// Date Creation: 29 August 2005
// Last Changing: 6 January 2006
// Author Rights By: Zie Technology, Zombie Soft And Lucky's Flash Studio Inc.,
// Licensed By: (C)DDD Digital Dreams Development And (C)3D Studios Inc., 2006
******************************************************************************/
#include <GRMethods.h>

//-----------------------------------------------------------------------------
// Name: FloatToDWord()
// Desc: Helper Function To Stuff A FLOAT Into A DWORD Argument
//-----------------------------------------------------------------------------
DWORD FloatToDWord(FLOAT F) 
{ 
  return *((DWORD*)&F); 
} //EndFloatToDWordFunction

//-----------------------------------------------------------------------------
// Name: ShowMessage()
// Desc: Rebuild From Delphi Syntax On C++
//-----------------------------------------------------------------------------
void ShowMessage(LPCSTR _MessageString)
{
  MessageBoxA(NULL,_MessageString,"GreenRay Engine",MB_SYSTEMMODAL|MB_ICONERROR);
} //EndShowErrorMessageProcedure

//-----------------------------------------------------------------------------
// Name: InitMaterial()
// Desc: Initializes A D3DMATERIAL9 Structure, Setting The Diffuse And Ambient
//       Colors. It Does Not Set Emissive Or Specular Colors
//-----------------------------------------------------------------------------
void InitMaterial(D3DMATERIAL9& _Mtrl,float r,float g,float b,float a)
{
  ZeroMemory(&_Mtrl,sizeof(D3DMATERIAL9));
  _Mtrl.Diffuse.r = _Mtrl.Ambient.r = r;
  _Mtrl.Diffuse.g = _Mtrl.Ambient.g = g;
  _Mtrl.Diffuse.b = _Mtrl.Ambient.b = b;
  _Mtrl.Diffuse.a = _Mtrl.Ambient.a = a;
} //EndInitMaterialProcedure

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

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction For CGRFPSCounter Class
///////////////////////////////////////////////////////////////////////////////
CGRFPSCounter::CGRFPSCounter(LPDIRECT3DDEVICE9 pDevice)
{
  d_pDevice = pDevice;
  m_fFPS = 0.0f; 

  //Create GreenRay FPS Counter Label Parameters
  m_pFPS = new CGRLabel(d_pDevice); //Construct A FPS Label
  m_pFPS->FontColor = clYellow; //Set Font Color
  m_pFPS->FontName = "Arial"; //Set Font Name
  m_pFPS->FontSize = 9; //Set Font Size
  m_pFPS->FontStyle = FS_BOLD; //Set Font Style
  m_pFPS->Left = 8;
  m_pFPS->Top = 4;
  m_pFPS->Update(); //Update Label Parameters

  m_pTimer = new CGRTimer(); //Construct A Timer Class
} //EndConstructionDirectives

CGRFPSCounter::~CGRFPSCounter()
{ 
} //EndDestructionDirectives

//-----------------------------------------------------------------------------
// Name: CGRFPSCounter::Release()
// Desc: 
//-----------------------------------------------------------------------------
void CGRFPSCounter::Release()
{
  d_pDevice = NULL;
  if (m_pFPS != NULL) { ReleaseClass(m_pFPS); DeleteClass(m_pFPS); } //Free FPS Font
  if (m_pTimer != NULL) { DeleteClass(m_pTimer); m_pTimer = NULL; } //Free Timer Class
} //EndReleaseProcedure

//-----------------------------------------------------------------------------
// Name: CGRFPSCounter::Render()
// Desc: 
//-----------------------------------------------------------------------------
void CGRFPSCounter::Render()
{
  if (m_pFPS != NULL) 
  {
    RefreshFPS(); //Refresh Frame Rate  
    m_pFPS->Caption(pFrameStats); //Show Frame Rate
  }
} //EndRenderProcedure

//-----------------------------------------------------------------------------
// Name: CGRFPSCounter::RefreshFPS()
// Desc: 
//-----------------------------------------------------------------------------
void CGRFPSCounter::RefreshFPS()
{
  //Keep Track Of The Frame Count
  //Timing Variality Directives
  static float fLastTime = 0.0f;
  static DWORD dwFrames = 0;
  
  double fTime = m_pTimer->GetAbsoluteTime();
    
  ++dwFrames;

  //Update The Scene Stats Once Per Second
  if (fTime-fLastTime > 1.0f)
  {
    m_fFPS = (float)dwFrames/(fTime-fLastTime);
    fLastTime = fTime;
    dwFrames = 0;

	TCHAR strFmt[100];
//        D3DFORMAT fmtAdapter = m_d3dSettings.DisplayMode().Format;
//        if( fmtAdapter == m_d3dsdBackBuffer.Format )
//        {
//            lstrcpyn( strFmt, D3DUtil_D3DFormatToString( fmtAdapter, false ), 100 );
//        }
//        else
//        {
//            _sntprintf( strFmt, 100, TEXT("backbuf %s, adapter %s"), 
//                D3DUtil_D3DFormatToString( m_d3dsdBackBuffer.Format, false ), 
//                D3DUtil_D3DFormatToString( fmtAdapter, false ) );
//        }
//        strFmt[99] = TEXT('\0');

        TCHAR strDepthFmt[100];
//        if( m_d3dEnumeration.AppUsesDepthBuffer )
//        {
//            _sntprintf( strDepthFmt, 100, TEXT(" (%s)"), 
//                D3DUtil_D3DFormatToString( m_d3dSettings.DepthStencilBufferFormat(), false ) );
//            strDepthFmt[99] = TEXT('\0');
//        }
//        else
//        {
            // No depth buffer
//            strDepthFmt[0] = TEXT('\0');
//        }

//        TCHAR* pstrMultiSample;
//        switch( m_d3dSettings.MultisampleType() )
//        {
//        case D3DMULTISAMPLE_NONMASKABLE: pstrMultiSample = TEXT(" (Nonmaskable Multisample)"); break;
//        case D3DMULTISAMPLE_2_SAMPLES:  pstrMultiSample = TEXT(" (2x Multisample)"); break;
//        case D3DMULTISAMPLE_3_SAMPLES:  pstrMultiSample = TEXT(" (3x Multisample)"); break;
//        case D3DMULTISAMPLE_4_SAMPLES:  pstrMultiSample = TEXT(" (4x Multisample)"); break;
//        case D3DMULTISAMPLE_5_SAMPLES:  pstrMultiSample = TEXT(" (5x Multisample)"); break;
//        case D3DMULTISAMPLE_6_SAMPLES:  pstrMultiSample = TEXT(" (6x Multisample)"); break;
//        case D3DMULTISAMPLE_7_SAMPLES:  pstrMultiSample = TEXT(" (7x Multisample)"); break;
//        case D3DMULTISAMPLE_8_SAMPLES:  pstrMultiSample = TEXT(" (8x Multisample)"); break;
//        case D3DMULTISAMPLE_9_SAMPLES:  pstrMultiSample = TEXT(" (9x Multisample)"); break;
//        case D3DMULTISAMPLE_10_SAMPLES: pstrMultiSample = TEXT(" (10x Multisample)"); break;
//        case D3DMULTISAMPLE_11_SAMPLES: pstrMultiSample = TEXT(" (11x Multisample)"); break;
//        case D3DMULTISAMPLE_12_SAMPLES: pstrMultiSample = TEXT(" (12x Multisample)"); break;
//        case D3DMULTISAMPLE_13_SAMPLES: pstrMultiSample = TEXT(" (13x Multisample)"); break;
//        case D3DMULTISAMPLE_14_SAMPLES: pstrMultiSample = TEXT(" (14x Multisample)"); break;
//        case D3DMULTISAMPLE_15_SAMPLES: pstrMultiSample = TEXT(" (15x Multisample)"); break;
//        case D3DMULTISAMPLE_16_SAMPLES: pstrMultiSample = TEXT(" (16x Multisample)"); break;
//        default:                        pstrMultiSample = TEXT(""); break;
//        }

  //Output D3D Information 
  const int cchMaxFrameStats = sizeof(pFrameStats)/sizeof(TCHAR);
  
  _sntprintf(pFrameStats,cchMaxFrameStats,_T("%.02f fps"/*(%dx%d), %s%s%s"*/),m_fFPS,
  0/*m_d3dsdBackBuffer.Width*/,0/*m_d3dsdBackBuffer.Height*/,strFmt,strDepthFmt,0/*pstrMultiSample*/);
  pFrameStats[cchMaxFrameStats-1] = TEXT('\0');
  }
} //EndRefreshFPSProcedure

/*
//Variables For Debugging
CGRFont* gr_Debug = NULL; //General GreenRay Debugger Font Interface
TCHAR* gr_RoomSt; //Vitia Room State
int gr_RoomStE; //Vitia Room Error State

CGRFont* gr_Axis = NULL; //Mouse Movement Control 


//-----------------------------------------------------------------------------
// Name: CreateFont()
// Desc: 
//-----------------------------------------------------------------------------
HRESULT CreateFont(LPDIRECT3DDEVICE9 pDevice,LPCSTR Param)
{


  if (Param == "Debug") //Create GreenRay Debugger Font Parameters
  {
    gr_Debug = new CGRFont(); //Construct A Debugger Font is Too 
    if (FAILED(gr_Debug->CreateD3DFont("Arial",FW_NORMAL,8))) return E_FAIL;
  }
  
  if (Param == "Axis") //Create Mouse Movement Font Parameters
  {
    gr_Axis = new CGRFont(); //Construct A Mouse Axis Movement Font 
    gr_Axis->SetTextColor(255,50,255,50); 
    if (FAILED(gr_Axis->CreateD3DFont("Arial",FW_NORMAL,8))) return E_FAIL;
  }
  return S_OK;

} //EndCreateFontFunction

//-----------------------------------------------------------------------------
// Name: ReleaseFont()
// Desc: 
//-----------------------------------------------------------------------------
void ReleaseFont(LPCSTR Param)
{
  //if (Param == "Debug") { if (gr_Debug != NULL) { ReleaseClass(gr_Debug); DeleteClass(gr_Debug); } } //Free Debug Font
  //if (Param == "Axis") { if (gr_Axis != NULL) { ReleaseClass(gr_Axis); DeleteClass(gr_Axis); } } //Free Mouse Axis Font
} //EndReleaseFontProcedure

//-----------------------------------------------------------------------------
// Name: OutputDebugInfo()
// Desc: 
//-----------------------------------------------------------------------------
void OutputDebugInfo()
{
  if (gr_Debug != NULL)
  {
    gr_Debug->OutputText(gr_RoomSt,10,68,gr_RoomStE); //Room Box Geometry DebugInfo
  }
} //EndOutputDebugInfoProcedure

//-----------------------------------------------------------------------------
// Name: OutputMouseXY()
// Desc: 
//-----------------------------------------------------------------------------
void OutputMouseXY(float X,float Y)
{
  if (gr_Axis != NULL)
  {
    TCHAR StrMouse[64]; 
    //Output Mouse AxisXY Coordinates   
    const int MouseState = sizeof(StrMouse)/sizeof(TCHAR);
    _sntprintf(StrMouse,MouseState,_T("Mouse Axis X: %0.03f; Y: %0.03f"),X,Y,0,0,0,0); //Formatting Float String
    StrMouse[MouseState-1] = TEXT('\0'); 
    gr_Axis->OutputText(StrMouse,10,45,4); 
  }
} //EndOutputMouseXYProcedure

//-----------------------------------------------------------------------------
// Name: OutputDisposition()
// Desc: 
//-----------------------------------------------------------------------------
void OutputDisposition(float X,float Y,float Z)
{ 
  if (gr_Axis != NULL)
  {
    TCHAR StrKeyboard[64]; 
    //Output Keyboard Coordinates   
    const int KeyboardState = sizeof(StrKeyboard)/sizeof(TCHAR);
    _sntprintf(StrKeyboard,KeyboardState,_T("Locate X: %0.03f; Y: %0.03f; Z: %0.03f"),X,Y,Z,0,0,0);
    StrKeyboard[KeyboardState-1] = TEXT('\0'); 
    gr_Axis->OutputText(StrKeyboard,10,55,3);
  }
} //EndOutputDispositionProcedure
*/

//-----------------------------------------------------------------------------