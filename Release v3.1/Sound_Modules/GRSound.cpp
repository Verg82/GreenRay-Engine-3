/*******************************************************************************
// It's Unofficial Version Of The GreenRay Engine v.1.0.0.2
// GRSound.cpp: 
// DirectSound Framework Classes For Reading And Writing Wav Files And
// Playing Them In DirectSound Buffers. Feel Free To Use This Class 
// As A Starting Point For Adding Extra Functionality.
// Date Creation: 29 August 2005
// Last Changing: 13 March 2006
// Author Rights By: Zie Technology, Zombie Soft And Lucky's Flash Studio Inc.,
// Licensed By: (C)DDD Digital Dreams Development Inc., 2006
*******************************************************************************/
#include <GRSound.h>
#include <GRMethods.h>

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction For TGRWave Class
///////////////////////////////////////////////////////////////////////////////
TGRWave::TGRWave()
{
  gr_SoundManager = NULL; 
  gr_Sound = NULL; 
} //EndConstructionDirectives

TGRWave::~TGRWave()
{ 
} //EndDestructionDirectives

//------------------------------------------------------------------------------
// Name: TGWave::CreateSoundInterface()
// Desc: Initialize The GreenRay Wave Interface For Playback Of Wave Files.
//------------------------------------------------------------------------------
HRESULT TGRWave::CreateSoundInterface(HWND _HWnd,TCHAR* FileName,
int Channels,int Frequency, int Bitrate)
{
  HRESULT _Result = S_OK;
  //Create A Static IDirectSound In The TGRSound Class.
  //Set CoopLevel To DSSCL_PRIORITY, And Set Primary Buffer Format To Stereo, 22kHz And 16-Bit Output.
  gr_SoundManager = new TGRSoundManager();
  if (FAILED(gr_SoundManager->Initialize(_HWnd,DSSCL_PRIORITY)))
  {
    gr_SoundManager = NULL;
	_Result = E_FAIL;
  }

  //Initializes The IDirectSound Object And Also Sets The Primary Buffer Format.  
  //This Function Must Be Called Before Any Others.
  if (FAILED(gr_SoundManager->SetPrimaryBufferFormat(Channels,Frequency,Bitrate)))
  {
    gr_SoundManager = NULL;
	_Result = E_FAIL;
  }
  
  //TODO: Load The Sounds From Resources (Or Files)
  if (FAILED(gr_SoundManager->Create(&gr_Sound,FileName,0,GUID_NULL,1/*Here Num Of Buffers*/)))
  {
    gr_Sound = NULL;
    gr_SoundManager = NULL;
	_Result = E_FAIL;
  }
  return _Result;
} //EndCreateSoundInterfaceFunction

//-----------------------------------------------------------------------------
// Name: TGWave::PlaybackSound()
// Desc: 
//-----------------------------------------------------------------------------
void TGRWave::PlaybackSound()
{
  gr_Sound->Play(0,0);
} //EndPlaybackSoundProcedure

//-----------------------------------------------------------------------------
// Name: TGWave::PlaybackReset()
// Desc: 
//-----------------------------------------------------------------------------
void TGRWave::PlaybackReset()
{
  gr_Sound->Reset();
  gr_Sound->Play(0,0);
} //EndPlaybackSoundProcedure

//-----------------------------------------------------------------------------
// Name: TGWave::PlaybackLooping()
// Desc: 
//-----------------------------------------------------------------------------
void TGRWave::PlaybackLooping()
{
  gr_Sound->Play(0,DSBPLAY_LOOPING);
} //EndPlaybackLoopingProcedure

//-----------------------------------------------------------------------------
// Name: TGWave::StopSound()
// Desc: 
//-----------------------------------------------------------------------------
void TGRWave::StopSound()
{
  gr_Sound->Stop();
} //EndStopSoundProcedure

//-----------------------------------------------------------------------------
// Name: TGWave::ReleaseSound()
// Desc: 
//-----------------------------------------------------------------------------
void TGRWave::ReleaseSound()
{
  //Release Sound
  if (gr_Sound != NULL) gr_Sound = NULL;
  if (gr_SoundManager != NULL) gr_SoundManager = NULL;
} //EndReleaseSoundProcedure

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction For CGRMusic Class
///////////////////////////////////////////////////////////////////////////////
CGRMusic::CGRMusic()
{
  //pBaseFilter = NULL;
  pGraphBuilder = NULL;
  pMediaControl = NULL;
  pMediaEventEx = NULL; 
  CoInitialize(NULL);
} //EndConstructionDirectives

CGRMusic::~CGRMusic()
{ 
  CoUninitialize();
} //EndDestructionDirectives

//-----------------------------------------------------------------------------
// Name: CGRMusic::CreateSound()
// Desc: 
//-----------------------------------------------------------------------------
HRESULT CGRMusic::CreateSound(LPCWSTR FileName)
{
  HRESULT _Result = S_OK;

  if (FAILED(CoCreateInstance(CLSID_FilterGraph,NULL,
  CLSCTX_INPROC_SERVER,IID_IGraphBuilder,(void**)&this->pGraphBuilder)))
  {
    ReleaseClass(pGraphBuilder);
	_Result = E_FAIL;
  }
  else
  {
    pGraphBuilder->QueryInterface(IID_IMediaControl,(void**)&pMediaControl);
    pGraphBuilder->QueryInterface(IID_IMediaEventEx,(void**)&pMediaEventEx); 
  }
  
  if (FAILED(pGraphBuilder->RenderFile(FileName,NULL)))
  {
	ShowMessage("GRSound.cpp: Can't Load Mp3 File Or Sound Initialization Error");
	ReleaseClass(pMediaControl);
	ReleaseClass(pMediaEventEx);
    ReleaseClass(pGraphBuilder);
	_Result = E_FAIL;
  }
  return _Result;
} //EndCreateSoundFunction

//-----------------------------------------------------------------------------
// Name: CGRMusic::PlaySound()
// Desc: 
//-----------------------------------------------------------------------------
void CGRMusic::PlaySound()
{
  if (pMediaControl != NULL) pMediaControl->Run();
} //EndPlaySoundProcedure

//-----------------------------------------------------------------------------
// Name: CGRMusic::IsUnplaying()
// Desc: 
//-----------------------------------------------------------------------------
BOOL CGRMusic::IsUnplaying()
{  
  long lEventCode,lParam1,lParam2;
  pMediaEventEx->GetEvent(&lEventCode,&lParam1,&lParam2,0);
  if (lEventCode == EC_COMPLETE) bUnplay = TRUE;  
  return bUnplay;
} //End

//-----------------------------------------------------------------------------
// Name: CGRMusic::PauseSound()
// Desc: 
//-----------------------------------------------------------------------------
void CGRMusic::PauseSound()
{
  if (pMediaControl != NULL) pMediaControl->Pause();
} //EndPauseSoundProcedure

//-----------------------------------------------------------------------------
// Name: CGRMusic::StopSound()
// Desc: 
//-----------------------------------------------------------------------------
void CGRMusic::StopSound()
{
  if (pMediaControl != NULL) pMediaControl->Stop();
} //EndStopSoundProcedure

//-----------------------------------------------------------------------------
// Name: CGRMusic::Release()
// Desc: 
//-----------------------------------------------------------------------------
void CGRMusic::Release()
{
  //if (pBaseFilter != NULL) ReleaseClass(pBaseFilter);
  if (pGraphBuilder != NULL) ReleaseClass(pGraphBuilder);
  if (pMediaControl != NULL) ReleaseClass(pMediaControl);
  if (pMediaEventEx != NULL) ReleaseClass(pMediaEventEx); 
} //EndReleaseProcedure

//////////////////////////////////////////////////////////////////////////////
// Construction/Destruction For TGRSoundManager Class
//////////////////////////////////////////////////////////////////////////////
TGRSoundManager::TGRSoundManager()
{
  m_pDirectSound = NULL;
} //EndConstructionDirectives

TGRSoundManager::~TGRSoundManager()
{
  ReleaseClass(m_pDirectSound); 
} //EndDestructionDirectives

//-----------------------------------------------------------------------------
// Name: TGSoundManager::Initialize()
// Desc: Initializes The IDirectSound Object And Also Sets The Primary Buffer
//       Format. This Function Must Be Called Before Any Others.
//-----------------------------------------------------------------------------
HRESULT TGRSoundManager::Initialize(HWND _HWnd,DWORD dwCoopLevel)
{    
  ReleaseClass(m_pDirectSound);

  //Create IDirectSound Using The Primary Sound Device
  if (FAILED(DirectSoundCreate8(NULL,&m_pDirectSound,NULL)))
  {
	ShowMessage("GRSound.cpp: Can't Create DirectSound Context");
	return E_FAIL;
  }

  //Set DirectSound Cooperative Level 
  if (FAILED(m_pDirectSound->SetCooperativeLevel(_HWnd,dwCoopLevel)))
  {
    ShowMessage("GRSound.cpp: Can't Create DirectSound CooperativeLevel");
	return E_FAIL;
  }  

  return S_OK;
} //EndInitializeFunction

//-----------------------------------------------------------------------------
// Name: TGRSoundManager::SetPrimaryBufferFormat()
// Desc: Set Primary Buffer To A Specified Format 
//       !WARNING! - Setting The Primary Buffer Format And Then Using This
//                   Same DSound Object For DirectMusic Messes Up DirectMusic! 
//       For Example, To Set The Primary Buffer Format To 22kHz Stereo, 16-Bit
//       Then: dwPrimaryChannels = 2
//             dwPrimaryFreq     = 22050 
//             dwPrimaryBitRate  = 16
//-----------------------------------------------------------------------------
HRESULT TGRSoundManager::SetPrimaryBufferFormat(DWORD dwPrimaryChannels, 
DWORD dwPrimaryFreq,DWORD dwPrimaryBitRate)
{
  HRESULT hr;
  LPDIRECTSOUNDBUFFER pDSBPrimary = NULL;

  if (m_pDirectSound == NULL) return CO_E_NOTINITIALIZED;

  //Get The Primary Buffer 
  DSBUFFERDESC _DSBD;
  ZeroMemory(&_DSBD,sizeof(DSBUFFERDESC));

  _DSBD.dwSize        = sizeof(DSBUFFERDESC);
  _DSBD.dwFlags       = DSBCAPS_PRIMARYBUFFER;
  _DSBD.dwBufferBytes = 0;
  _DSBD.lpwfxFormat   = NULL;
       
  if (FAILED(m_pDirectSound->CreateSoundBuffer(&_DSBD,&pDSBPrimary,NULL)))
  {
	ShowMessage("GRSound.cpp: Can't Create DirectSound Buffer"); 
	return E_FAIL; 
  }
  
  //Get The Wave Format
  WAVEFORMATEX _WFX;
  ZeroMemory(&_WFX,sizeof(WAVEFORMATEX));
   
  _WFX.wFormatTag      = (WORD)WAVE_FORMAT_PCM; 
  _WFX.nChannels       = (WORD)dwPrimaryChannels; 
  _WFX.nSamplesPerSec  = (DWORD)dwPrimaryFreq; 
  _WFX.wBitsPerSample  = (WORD)dwPrimaryBitRate; 
  _WFX.nBlockAlign     = (WORD)(_WFX.wBitsPerSample/8*_WFX.nChannels);
  _WFX.nAvgBytesPerSec = (DWORD)(_WFX.nSamplesPerSec*_WFX.nBlockAlign);

  if (FAILED(hr = pDSBPrimary->SetFormat(&_WFX)))
  {
	ShowMessage("GRSound.cpp: Can't Set Format Of DirectSound Primary Buffer"); 
    return E_FAIL;
  }
  
  ReleaseClass(pDSBPrimary);
  return S_OK;
} //EndSetPrimaryBufferFormatFunction

//-----------------------------------------------------------------------------
// Name: TGRSoundManager::Create()
// Desc: 
//-----------------------------------------------------------------------------
HRESULT TGRSoundManager::Create(TGRSound** ppSound,LPTSTR strWaveFileName, 
DWORD dwCreationFlags,GUID _GUID3DAlgorithm,DWORD dwNumBuffers)
{
  HRESULT hr;
  HRESULT hrRet = S_OK;

  DWORD I;
  LPDIRECTSOUNDBUFFER* apDSBuffer = NULL;
  DWORD dwDSBufferSize = NULL;
  TGRWaveFile* pWaveFile = NULL;

  if (m_pDirectSound == NULL) return CO_E_NOTINITIALIZED;
  if (strWaveFileName == NULL||ppSound == NULL||dwNumBuffers < 1) return E_INVALIDARG;

  apDSBuffer = new LPDIRECTSOUNDBUFFER[dwNumBuffers];
  if (apDSBuffer == NULL)
  {
    hr = E_OUTOFMEMORY;
    goto LFailed;
  }

  pWaveFile = new TGRWaveFile();
  if (pWaveFile == NULL)
  {
    hr = E_OUTOFMEMORY;
    goto LFailed;
  }

  pWaveFile->Open(strWaveFileName,NULL,WAVEFILE_READ);

  if( pWaveFile->GetSize() == 0 )
  {
    //Wave Is Blank, So Don't Create It.
    hr = E_FAIL;
    goto LFailed;
  }

  //Make The DirectSound Buffer The Same Size As The Wav File
  dwDSBufferSize = pWaveFile->GetSize();

  //Create The Direct Sound Buffer, And Only Request The Flags Needed
  //Since Each Requires Some Overhead And Limits If The Buffer Can 
  //Be Hardware Accelerated.
  DSBUFFERDESC _DSBD;
  ZeroMemory(&_DSBD,sizeof(DSBUFFERDESC));
    
  _DSBD.dwSize          = sizeof(DSBUFFERDESC);
  _DSBD.dwFlags         = dwCreationFlags;
  _DSBD.dwBufferBytes   = dwDSBufferSize;
  _DSBD.guid3DAlgorithm = _GUID3DAlgorithm;
  _DSBD.lpwfxFormat     = pWaveFile->m_pwfx;
    
  //DirectSound Is Only Guarenteed To Play PCM Data. Other
  //Formats May Or May Not Work Depending The Sound Card Driver.
  hr = m_pDirectSound->CreateSoundBuffer(&_DSBD,&apDSBuffer[0],NULL);

  //Be Sure To Return This Error Code If It Occurs So The
  //Callers Knows This Happened.
  if (hr == DS_NO_VIRTUALIZATION) hrRet = DS_NO_VIRTUALIZATION;
            
  if (FAILED(hr))
  {
    //DSERR_BUFFERTOOSMALL Will Be Returned If The Buffer Is
    //Less Than DSBSIZE_FX_MIN And The Buffer Is Created
    //With DSBCAPS_CTRLFX.
        
    //It Might Also Fail If Hardware Buffer Mixing Was Requested
    //On A Device That Doesn't Support It.
	ShowMessage("GRSound.cpp: Error Creation Sound Buffer");
	//hr;                    
    goto LFailed;
  }

  //Default To Use DuplicateSoundBuffer() When Created Extra Buffers Since Always 
  //Create A Buffer That Uses The Same Memory However DuplicateSoundBuffer() Will Fail If 
  //DSBCAPS_CTRLFX Is Used, So Use CreateSoundBuffer() Instead In This Case.
  if ((dwCreationFlags & DSBCAPS_CTRLFX) == 0)
  {
    for (I = 1; I<dwNumBuffers; I++)
    {
      if (FAILED(hr = m_pDirectSound->DuplicateSoundBuffer(apDSBuffer[0],&apDSBuffer[I])))
      {
	    ShowMessage("GRSound.cpp: Duplication Sound Buffer");
        //hr;
        goto LFailed;
      }
    } //EndFor
  } //EnddwCreationFlags
  else
  {
    for (I = 1; I<dwNumBuffers; I++)
    {
      hr = m_pDirectSound->CreateSoundBuffer(&_DSBD,&apDSBuffer[I],NULL);
      if (FAILED(hr))
      {
	    ShowMessage("GRSound.cpp: Error Creation Sound Buffer");
        //hr;
        goto LFailed;
      }
    }//EndFor
  } //EndElse
    
  //Create The Sound
  *ppSound = new TGRSound(apDSBuffer,dwDSBufferSize,dwNumBuffers,pWaveFile,dwCreationFlags);
    
  DeleteClass(apDSBuffer);
  return hrRet;

LFailed:
  //Cleanup
  DeleteClass(pWaveFile);
  DeleteClass(apDSBuffer);
  return hr;
} //EndCreateFunction

//-----------------------------------------------------------------------------
// Name: TGRSoundManager::CreateFromMemory()
// Desc: 
//-----------------------------------------------------------------------------
HRESULT TGRSoundManager::CreateFromMemory(TGRSound** ppSound,BYTE* pbData,ULONG ulDataSize,
LPWAVEFORMATEX pwfx,DWORD dwCreationFlags,GUID _GUID3DAlgorithm,DWORD dwNumBuffers)
{
  HRESULT hr;
  DWORD I;
  LPDIRECTSOUNDBUFFER* apDSBuffer = NULL;
  DWORD dwDSBufferSize = NULL;
  TGRWaveFile* pWaveFile = NULL;

  if (m_pDirectSound == NULL) return CO_E_NOTINITIALIZED;
  if (pbData == NULL||ppSound == NULL||dwNumBuffers < 1) return E_INVALIDARG;

  apDSBuffer = new LPDIRECTSOUNDBUFFER[dwNumBuffers];
  if (apDSBuffer == NULL)
  {
    hr = E_OUTOFMEMORY;
    goto LFailed;
  }

  pWaveFile = new TGRWaveFile();
  if( pWaveFile == NULL )
  {
    hr = E_OUTOFMEMORY;
    goto LFailed;
  }

  pWaveFile->OpenFromMemory(pbData,ulDataSize,pwfx,WAVEFILE_READ);

  //Make The DirectSound Buffer The Same Size As The Wav File
  dwDSBufferSize = ulDataSize;

  //Create The Direct Sound Buffer, And Only Request The Flags Needed
  //Since Each Requires Some Overhead And Limits If The Buffer Can 
  //Be Hardware Accelerated
  DSBUFFERDESC _DSBD;
  ZeroMemory(&_DSBD,sizeof(DSBUFFERDESC));
  
  _DSBD.dwSize          = sizeof(DSBUFFERDESC);
  _DSBD.dwFlags         = dwCreationFlags;
  _DSBD.dwBufferBytes   = dwDSBufferSize;
  _DSBD.guid3DAlgorithm = _GUID3DAlgorithm;
  _DSBD.lpwfxFormat     = pwfx;

  if (FAILED(hr = m_pDirectSound->CreateSoundBuffer(&_DSBD,&apDSBuffer[0],NULL)))
  {
    hr;
    goto LFailed;
  }

  //Default To Use DuplicateSoundBuffer() When Created Extra Buffers Since Always 
  //Create A Buffer That Uses The Same Memory However DuplicateSoundBuffer() Will Fail If 
  //DSBCAPS_CTRLFX Is Used, So Use CreateSoundBuffer() Instead In This Case.
  if ((dwCreationFlags & DSBCAPS_CTRLFX) == 0)
  {
    for(I = 1; I<dwNumBuffers; I++)
    {
      if (FAILED(hr = m_pDirectSound->DuplicateSoundBuffer(apDSBuffer[0],&apDSBuffer[I])))
      {
        hr;
        goto LFailed;
       }
    } //EndFor
  } //EnddwCreationFlags
  else
  {
    for (I = 1; I<dwNumBuffers; I++)
    {
      hr = m_pDirectSound->CreateSoundBuffer(&_DSBD,&apDSBuffer[I],NULL);
      if (FAILED(hr))
      {
        hr;
        goto LFailed;
      }
    } //EndFor
  } //EndElse

  //Create The Sound
  *ppSound = new TGRSound(apDSBuffer,dwDSBufferSize,dwNumBuffers,pWaveFile,dwCreationFlags);

  DeleteClass(apDSBuffer);
  return S_OK;

LFailed:
  //Cleanup
  DeleteClass(apDSBuffer);
  return hr;
} //EndCreateFromMemoryFunction

//-----------------------------------------------------------------------------
// Name: TGRSoundManager::CreateStreaming()
// Desc: 
//-----------------------------------------------------------------------------
HRESULT TGRSoundManager::CreateStreaming(TGRStreamingSound** ppStreamingSound,LPTSTR strWaveFileName, 
DWORD dwCreationFlags,GUID _GUID3DAlgorithm,DWORD dwNotifyCount,DWORD dwNotifySize,HANDLE hNotifyEvent)
{
  HRESULT hr;

  if (m_pDirectSound == NULL) return CO_E_NOTINITIALIZED;
  if (strWaveFileName == NULL||ppStreamingSound == NULL||hNotifyEvent == NULL) return E_INVALIDARG;

  LPDIRECTSOUNDBUFFER pDSBuffer = NULL;
  DWORD dwDSBufferSize = NULL;
  TGRWaveFile* pWaveFile = NULL;
  DSBPOSITIONNOTIFY* aPosNotify = NULL; 
  LPDIRECTSOUNDNOTIFY pDSNotify = NULL;

  pWaveFile = new TGRWaveFile();
  if (pWaveFile == NULL) return E_OUTOFMEMORY;
  pWaveFile->Open(strWaveFileName,NULL,WAVEFILE_READ);

  //Figure Out How Big The DSound Buffer Should Be 
  dwDSBufferSize = dwNotifySize*dwNotifyCount;

  //Set Up The Direct Sound Buffer. Request The NOTIFY Flag, So
  //That We Are Notified As The Sound Buffer Plays. Note, That Using This Flag
  //May Limit The Amount Of Hardware Acceleration That Can Occur. 
  DSBUFFERDESC _DSBD;
  ZeroMemory(&_DSBD,sizeof(DSBUFFERDESC));

  _DSBD.dwSize          = sizeof(DSBUFFERDESC);
  _DSBD.dwFlags         = dwCreationFlags|DSBCAPS_CTRLPOSITIONNOTIFY|DSBCAPS_GETCURRENTPOSITION2;
  _DSBD.dwBufferBytes   = dwDSBufferSize;
  _DSBD.guid3DAlgorithm = _GUID3DAlgorithm;
  _DSBD.lpwfxFormat     = pWaveFile->m_pwfx;

  if (FAILED(hr = m_pDirectSound->CreateSoundBuffer(&_DSBD,&pDSBuffer,NULL)))
  {
    //If Wave Format Isn't Then It Will Return 
    //Either DSERR_BADFORMAT or E_INVALIDARG
    if (hr == DSERR_BADFORMAT||hr == E_INVALIDARG) return hr;
    return hr;
  }

  //Create The Notification Events, So That We Know When To Fill
  //The Buffer As The Sound Plays. 
  if (FAILED(hr = pDSBuffer->QueryInterface(IID_IDirectSoundNotify,(VOID**)&pDSNotify)))
  {
    DeleteArray(aPosNotify);
    return hr;
  }

  aPosNotify = new DSBPOSITIONNOTIFY[dwNotifyCount];
  if (aPosNotify == NULL) return E_OUTOFMEMORY;

  for (DWORD I = 0; I<dwNotifyCount; I++)
  {
    aPosNotify[I].dwOffset = (dwNotifySize*I)+dwNotifySize-1;
    aPosNotify[I].hEventNotify = hNotifyEvent;             
  }
    
  //Tell DirectSound When To Notify Us. The Notification Will Come In The From 
  //Of Signaled Events That Are Handled In WinMain()
  if (FAILED(hr = pDSNotify->SetNotificationPositions(dwNotifyCount,aPosNotify)))
  {
    ReleaseClass(pDSNotify);
    DeleteArray(aPosNotify);
    return hr;
  }

  ReleaseClass(pDSNotify);
  DeleteArray(aPosNotify);

  //Create The Sound
  *ppStreamingSound = new TGRStreamingSound(pDSBuffer,dwDSBufferSize,pWaveFile,dwNotifySize);

  return S_OK;
} //EndCreateStreamingFunction

//-----------------------------------------------------------------------------
// Name: TGRSoundManager::Get3DListenerInterface()
// Desc: Returns The 3D Listener Interface Associated With Primary Buffer.
//-----------------------------------------------------------------------------
HRESULT TGRSoundManager::Get3DListenerInterface(LPDIRECTSOUND3DLISTENER* ppDSListener)
{
  HRESULT hr;
  DSBUFFERDESC _BuffDesc;
  LPDIRECTSOUNDBUFFER pDSBPrimary = NULL;

  if (ppDSListener == NULL) return E_INVALIDARG;
  if (m_pDirectSound == NULL) return CO_E_NOTINITIALIZED;

  *ppDSListener = NULL;

  //Obtain Primary Buffer, Asking It For 3D Control
  ZeroMemory(&_BuffDesc,sizeof(DSBUFFERDESC));
    
  _BuffDesc.dwSize = sizeof(DSBUFFERDESC);
  _BuffDesc.dwFlags = DSBCAPS_CTRL3D|DSBCAPS_PRIMARYBUFFER;
  
  if (FAILED(hr = m_pDirectSound->CreateSoundBuffer(&_BuffDesc,&pDSBPrimary,NULL))) return hr;

  if (FAILED(hr = pDSBPrimary->QueryInterface(IID_IDirectSound3DListener,(VOID**)ppDSListener)))
  {
    ReleaseClass(pDSBPrimary);
    return hr ;
  }

  //Release The Primary Buffer, Since It Is Not Need Anymore
  ReleaseClass(pDSBPrimary);

  return S_OK;
} //EndGet3DListenerInterfaceFunction

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction For TGRWaveFile Class
///////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
// Name: TGTWaveFile::TGRWaveFile()
// Desc: Constructs The Class. Call Open() To Open A Wave File For Reading.  
//       Then Call Read() As Needed. Calling The Destructor Or Close() 
//       Will Close The File.  
//-----------------------------------------------------------------------------
TGRWaveFile::TGRWaveFile()
{
   m_pwfx = NULL;
   m_hmmio = NULL;
   m_pResourceBuffer = NULL;
   m_dwSize = 0;
   m_bIsReadingFromMemory = FALSE;
} //EndConstructionDirectives

//-----------------------------------------------------------------------------
// Name: TGRWaveFile::~TGRWaveFile()
// Desc: Destructs The Class
//-----------------------------------------------------------------------------
TGRWaveFile::~TGRWaveFile()
{
  Close();
  if (!m_bIsReadingFromMemory) DeleteArray(m_pwfx);
} //EndDestructionDirectives

//-----------------------------------------------------------------------------
// Name: TGRWaveFile::Open()
// Desc: Opens A Wave File For Reading
//-----------------------------------------------------------------------------
HRESULT TGRWaveFile::Open(LPTSTR strFileName,WAVEFORMATEX* pwfx,DWORD dwFlags)
{
  HRESULT hr;

  m_dwFlags = dwFlags;
  m_bIsReadingFromMemory = FALSE;

  if (m_dwFlags == WAVEFILE_READ)
  {
    if (strFileName == NULL) return E_INVALIDARG;
    DeleteArray(m_pwfx);
    m_hmmio = mmioOpen(strFileName,NULL,MMIO_ALLOCBUF|MMIO_READ);

    if (NULL == m_hmmio)
    {
      HRSRC hResInfo;
      HGLOBAL hResData;
      DWORD dwSize;
      VOID* pvRes;

      //Loading It As A File Failed, So Try It As A Resource
      if (NULL == (hResInfo = FindResource(NULL,strFileName,TEXT("WAVE"))))
      {
        if (NULL == (hResInfo = FindResource(NULL,strFileName,TEXT("WAV")))) return E_FAIL;
      }

      if (NULL == (hResData = LoadResource(NULL,hResInfo))) return E_FAIL;
      if (0 == (dwSize = SizeofResource(NULL,hResInfo))) return E_FAIL;
      if (NULL == (pvRes = LockResource(hResData))) return E_FAIL;

      m_pResourceBuffer = new CHAR[dwSize];
      memcpy(m_pResourceBuffer,pvRes,dwSize);

      MMIOINFO _MMIOInfo;
      ZeroMemory(&_MMIOInfo,sizeof(_MMIOInfo));

      _MMIOInfo.fccIOProc = FOURCC_MEM;
      _MMIOInfo.cchBuffer = dwSize;
      _MMIOInfo.pchBuffer = (CHAR*)m_pResourceBuffer;

      m_hmmio = mmioOpen(NULL,&_MMIOInfo,MMIO_ALLOCBUF|MMIO_READ);
    }

    if (FAILED(hr = ReadMMIO()))
    {
      //ReadMMIO Will Fail If It's An Not A Wave File
      mmioClose(m_hmmio,0);
      return hr;
    }

    if (FAILED(hr = ResetFile())) return hr;

    //After The Reset, The Size Of The Wav File Is m_ck.cksize So Store It Now
    m_dwSize = m_ck.cksize;
  } //End m_dwFlags
  else
  {
    m_hmmio = mmioOpen(strFileName,NULL,MMIO_ALLOCBUF|MMIO_READWRITE|MMIO_CREATE);
    if (NULL == m_hmmio) return E_FAIL;
    if (FAILED(hr = WriteMMIO(pwfx)))
    {
      mmioClose(m_hmmio,0);
      return hr;
    }
                        
    if (FAILED(hr = ResetFile())) return hr;
  }

  return hr;
} //EndTGRWaveFileOpenFunction

//-----------------------------------------------------------------------------
// Name: TGRWaveFile::OpenFromMemory()
// Desc: Copy Data To TGRWaveFile Member Variable From Memory
//-----------------------------------------------------------------------------
HRESULT TGRWaveFile::OpenFromMemory(BYTE* pbData,ULONG ulDataSize,WAVEFORMATEX* pwfx,DWORD dwFlags)
{
  m_pwfx = pwfx;
  m_ulDataSize = ulDataSize;
  m_pbData = pbData;
  m_pbDataCur = m_pbData;
  m_bIsReadingFromMemory = TRUE;
    
  if (dwFlags != WAVEFILE_READ) return E_NOTIMPL;       
  
  return S_OK;
} //EndOpenFromMemoryFunction

//-----------------------------------------------------------------------------
// Name: TGRWaveFile::ResetFile()
// Desc: Resets The Internal m_ck Pointer So Reading Starts From The 
//       Beginning Of The File Again 
//-----------------------------------------------------------------------------
HRESULT TGRWaveFile::ResetFile()
{
  if (m_bIsReadingFromMemory)
  {
    m_pbDataCur = m_pbData;
  }
  else 
  {
    if (m_hmmio == NULL) return CO_E_NOTINITIALIZED;
    if (m_dwFlags == WAVEFILE_READ)
    {
      //Seek To The Data
      if (-1 == mmioSeek(m_hmmio,m_ckRiff.dwDataOffset+sizeof(FOURCC),SEEK_SET)) return E_FAIL;

      //Search The Input File For The 'data' Chunk.
      m_ck.ckid = mmioFOURCC('d','a','t','a');
      if (0 != mmioDescend(m_hmmio,&m_ck,&m_ckRiff,MMIO_FINDCHUNK)) return E_FAIL;
    }
    else
    {
      //Create The 'data' Chunk That Holds The Waveform Samples.  
      m_ck.ckid = mmioFOURCC('d','a','t','a');
      m_ck.cksize = 0;

      if (0 != mmioCreateChunk(m_hmmio,&m_ck,0)) return E_FAIL;
      if (0 != mmioGetInfo(m_hmmio,&m_mmioinfoOut,0)) return E_FAIL;
    }
  } //End Else
  return S_OK;
} //EndResetFileFunction

//-----------------------------------------------------------------------------
// Name: TGRWaveFile::GetSize()
// Desc: Retuns The Size Of The Read Access Wave File 
//-----------------------------------------------------------------------------
DWORD TGRWaveFile::GetSize()
{
  return m_dwSize;
} //EndGetSizeFunction

//-----------------------------------------------------------------------------
// Name: TGRWaveFile::Read()
// Desc: Reads Section Of Data From A Wave File Into pBuffer And Returns 
//       How Much Read In pdwSizeRead, Reading Not More Than dwSizeToRead.
//       This Uses m_ck To Determine Where To Start Reading From. So 
//       Subsequent Calls Will Be Continue Where The Last Left Off Unless 
//       Reset() Is Called.
//-----------------------------------------------------------------------------
HRESULT TGRWaveFile::Read(BYTE* pBuffer,DWORD dwSizeToRead,DWORD* pdwSizeRead)
{
  if (m_bIsReadingFromMemory)
  {
    if (m_pbDataCur == NULL) return CO_E_NOTINITIALIZED;
    if (pdwSizeRead != NULL) *pdwSizeRead = 0;
	if ((BYTE*)(m_pbDataCur+dwSizeToRead) > (BYTE*)(m_pbData+m_ulDataSize))
    {
      dwSizeToRead = m_ulDataSize-(DWORD)(m_pbDataCur-m_pbData);
    }
        
    CopyMemory(pBuffer,m_pbDataCur,dwSizeToRead);
    if (pdwSizeRead != NULL) *pdwSizeRead = dwSizeToRead;

    return S_OK;
  }
  else 
  {
    MMIOINFO mmioinfoIn; //Current Status Of m_hmmio

    if (m_hmmio == NULL) return CO_E_NOTINITIALIZED;
    if (pBuffer == NULL||pdwSizeRead == NULL) return E_INVALIDARG;
    if (pdwSizeRead != NULL) *pdwSizeRead = 0;
    if (0 != mmioGetInfo(m_hmmio,&mmioinfoIn,0)) return E_FAIL;
                
    UINT cbDataIn = dwSizeToRead;
    if (cbDataIn > m_ck.cksize) cbDataIn = m_ck.cksize;       

    m_ck.cksize-= cbDataIn;
    
    for (DWORD cT = 0; cT<cbDataIn; cT++)
    {
      //Copy The Bytes From The IO To The Buffer.
      if (mmioinfoIn.pchNext == mmioinfoIn.pchEndRead)
      {
        if (0 != mmioAdvance(m_hmmio,&mmioinfoIn,MMIO_READ)) return E_FAIL;
        if (mmioinfoIn.pchNext == mmioinfoIn.pchEndRead) return E_FAIL;
      }

      //Actual Copy
      *((BYTE*)pBuffer+cT) = *((BYTE*)mmioinfoIn.pchNext);
      mmioinfoIn.pchNext++;
    }

    if (0 != mmioSetInfo(m_hmmio,&mmioinfoIn,0)) return E_FAIL;
    if (pdwSizeRead != NULL) *pdwSizeRead = cbDataIn;

    return S_OK;
  } //End Else
} //EndTGRWaveFileReadFunction


//-----------------------------------------------------------------------------
// Name: TGRWaveFile::Write()
// Desc: Writes Data To The Open Wave File
//-----------------------------------------------------------------------------
HRESULT TGRWaveFile::Write(UINT nSizeToWrite,BYTE* pbSrcData,UINT* pnSizeWrote)
{
  UINT cT;

  if (m_bIsReadingFromMemory) return E_NOTIMPL;
  if (m_hmmio == NULL) return CO_E_NOTINITIALIZED;
  if (pnSizeWrote == NULL||pbSrcData == NULL) return E_INVALIDARG;

  *pnSizeWrote = 0;
    
  for (cT = 0; cT<nSizeToWrite; cT++)
  {       
    if (m_mmioinfoOut.pchNext == m_mmioinfoOut.pchEndWrite)
    {
      m_mmioinfoOut.dwFlags |= MMIO_DIRTY;
      if (0 != mmioAdvance(m_hmmio,&m_mmioinfoOut,MMIO_WRITE)) return E_FAIL;
    }

    *((BYTE*)m_mmioinfoOut.pchNext) = *((BYTE*)pbSrcData+cT);
    (BYTE*)m_mmioinfoOut.pchNext++;
    (*pnSizeWrote)++;
  }
  return S_OK;
} //EndTGRWaveFileWriteFunction

//-----------------------------------------------------------------------------
// Name: TGRWaveFile::Close()
// Desc: Closes The Wave File 
//-----------------------------------------------------------------------------
HRESULT TGRWaveFile::Close()
{
  if (m_dwFlags == WAVEFILE_READ)
  {
    mmioClose(m_hmmio,0);
    m_hmmio = NULL;
    DeleteArray(m_pResourceBuffer);
  }
  else
  {
    m_mmioinfoOut.dwFlags |= MMIO_DIRTY;
    if (m_hmmio == NULL) return CO_E_NOTINITIALIZED;
    if (0 != mmioSetInfo(m_hmmio,&m_mmioinfoOut,0)) return E_FAIL;
    
    //Ascend The Output File Out Of The 'data' Chunk -- This Will Cause
    //The Chunk Size Of The 'data' Chunk To Be Written.
    if (0 != mmioAscend(m_hmmio,&m_ck,0)) return E_FAIL;
    
    //Do This Here Instead...
    if (0 != mmioAscend(m_hmmio,&m_ckRiff,0)) return E_FAIL;
    mmioSeek(m_hmmio,0,SEEK_SET);

    if (0 != (INT)mmioDescend(m_hmmio,&m_ckRiff,NULL,0)) return E_FAIL;
    m_ck.ckid = mmioFOURCC('f','a','c','t');

    if (0 == mmioDescend(m_hmmio,&m_ck,&m_ckRiff,MMIO_FINDCHUNK)) 
    {
      DWORD dwSamples = 0;
      mmioWrite(m_hmmio,(HPSTR)&dwSamples,sizeof(DWORD));
      mmioAscend(m_hmmio,&m_ck,0); 
    }
    
    //Ascend The Output File Out Of The 'RIFF' Chunk -- This Will Cause
    //The Chunk Size Of The 'RIFF' Chunk To Be Written.
    if (0 != mmioAscend(m_hmmio,&m_ckRiff,0)) return E_FAIL;
    
    mmioClose(m_hmmio,0);
    m_hmmio = NULL;
  }
  return S_OK;
} //EndTGRWaveFileCloseFunction

//-----------------------------------------------------------------------------
// Name: TGRWaveFile::ReadMMIO()
// Desc: Support Function For Reading From A Multimedia I/O Stream.
//       m_hmmio Must Be Valid Before Calling. This Function Uses It To
//       Update m_ckRiff, And m_pwfx. 
//-----------------------------------------------------------------------------
HRESULT TGRWaveFile::ReadMMIO()
{
  MMCKINFO ckIn; //Chunk Info For General Us.
  PCMWAVEFORMAT pcmWaveFormat; //Temp PCM Structure To Load In       

  m_pwfx = NULL;

  if ((0 != mmioDescend(m_hmmio,&m_ckRiff,NULL,0))) return E_FAIL;

  //Check To Make Sure This Is A Valid Wave File
  if ((m_ckRiff.ckid != FOURCC_RIFF) || (m_ckRiff.fccType != mmioFOURCC('W','A','V','E'))) return E_FAIL; 

  //Search The Input File For The 'fmt ' Chunk
  ckIn.ckid = mmioFOURCC('f','m','t',' ');
  if (0 != mmioDescend(m_hmmio,&ckIn,&m_ckRiff,MMIO_FINDCHUNK)) return E_FAIL;

  //Expect The 'fmt' Chunk To Be At Least As Large As <PCMWAVEFORMAT>
  //If There Are Extra Parameters At The End, We'll Ignore Them
  if (ckIn.cksize < (LONG)sizeof(PCMWAVEFORMAT)) return E_FAIL;

  //Read The 'fmt ' Chunk Into <pcmWaveFormat>
  if (mmioRead(m_hmmio,(HPSTR)&pcmWaveFormat,sizeof(pcmWaveFormat)) != sizeof(pcmWaveFormat)) return E_FAIL;

  //Allocate The Waveformatex, But If It's Not PCM Format, Read The Next
  //Word, And Thats How Many Extra Bytes To Allocate.
  if (pcmWaveFormat.wf.wFormatTag == WAVE_FORMAT_PCM)
  {
    m_pwfx = (WAVEFORMATEX*)new CHAR[sizeof(WAVEFORMATEX)];
    if (NULL == m_pwfx) return E_FAIL;

    //Copy The Bytes From The PCM Structure To The Waveformatex Structure
    memcpy(m_pwfx,&pcmWaveFormat,sizeof(pcmWaveFormat));
    m_pwfx->cbSize = 0;
  }
  else
  {
    //Read In Length Of Extra Bytes.
    WORD cbExtraBytes = 0L;
    if (mmioRead(m_hmmio,(CHAR*)&cbExtraBytes,sizeof(WORD)) != sizeof(WORD)) return E_FAIL;

    m_pwfx = (WAVEFORMATEX*)new CHAR[sizeof(WAVEFORMATEX)+cbExtraBytes];
    if (NULL == m_pwfx) return E_FAIL;

    //Copy The Bytes From The PCM Structure To The Waveformatex Structure
    memcpy(m_pwfx,&pcmWaveFormat,sizeof(pcmWaveFormat));
    m_pwfx->cbSize = cbExtraBytes;

    //Now, Read Those Extra Bytes Into The Structure, If cbExtraAlloc != 0
    if (mmioRead(m_hmmio,(CHAR*)(((BYTE*)&(m_pwfx->cbSize))+sizeof(WORD)),cbExtraBytes) != cbExtraBytes)
    {
      DeleteClass(m_pwfx);
      return E_FAIL;
    }
  } //End Else

  //Ascend The Input File Out Of The 'fmt ' Chunk
  if (0 != mmioAscend(m_hmmio,&ckIn,0))
  {
    DeleteClass(m_pwfx);
    return E_FAIL;
  }
  return S_OK;
} //EndReadMMIOFunction

//-----------------------------------------------------------------------------
// Name: TGRWaveFile::WriteMMIO()
// Desc: Support Function For Reading From A Multimedia I/O Stream
//       pwfxDest Is The WAVEFORMATEX For This New Wave File.  
//       m_hmmio Must Be Valid Before Calling. This Function Uses It To
//       Update m_ckRiff, And m_ck.  
//-----------------------------------------------------------------------------
HRESULT TGRWaveFile::WriteMMIO(WAVEFORMATEX *pwfxDest)
{
  DWORD dwFactChunk; //Contains The Actual Fact Chunk. Garbage Until WaveCloseWriteFile.
  MMCKINFO ckOut1;
    
  dwFactChunk = (DWORD)-1;

  //Create The Output File RIFF Chunk Of Form Type 'WAVE'.
  m_ckRiff.fccType = mmioFOURCC('W','A','V','E');       
  m_ckRiff.cksize = 0;

  if (0 != mmioCreateChunk(m_hmmio,&m_ckRiff,MMIO_CREATERIFF)) return E_FAIL;
    
  //We Are Now Descended Into The 'RIFF' Chunk We Just Created.
  //Now Create The 'fmt ' Chunk. Since We Know The Size Of This Chunk,
  //Specify It In The MMCKINFO Structure So MMIO Doesn't Have To Seek
  //Back And Set The Chunk Size After Ascending From The Chunk..
  m_ck.ckid = mmioFOURCC('f','m','t',' ');
  m_ck.cksize = sizeof(PCMWAVEFORMAT);   

  if (0 != mmioCreateChunk(m_hmmio,&m_ck,0)) return E_FAIL;
    
  //Write The PCMWAVEFORMAT Structure To The 'fmt ' Chunk If It's That Type..
  if (pwfxDest->wFormatTag == WAVE_FORMAT_PCM)
  {
    if (mmioWrite(m_hmmio,(HPSTR)pwfxDest,sizeof(PCMWAVEFORMAT)) != sizeof(PCMWAVEFORMAT)) return E_FAIL;
  }   
  else 
  {
    //Write The Variable Length Size..
    if ((UINT)mmioWrite(m_hmmio,(HPSTR)pwfxDest,
    sizeof(*pwfxDest)+pwfxDest->cbSize) != (sizeof(*pwfxDest)+pwfxDest->cbSize)) return E_FAIL;
  }  
    
  //Ascend Out Of The 'fmt ' Chunk, Back Into The 'RIFF' Chunk..
  if (0 != mmioAscend(m_hmmio,&m_ck,0)) return E_FAIL;
    
  //Now Create The Fact Chunk, Not Required For PCM But Nice To Have. This Is Filled
  //In When The Close Routine Is Called..
  ckOut1.ckid = mmioFOURCC('f','a','c','t');
  ckOut1.cksize = 0;

  if (0 != mmioCreateChunk(m_hmmio,&ckOut1,0)) return E_FAIL;
  if (mmioWrite(m_hmmio,(HPSTR)&dwFactChunk,sizeof(dwFactChunk)) != sizeof(dwFactChunk)) return E_FAIL;
    
  //Now Ascend Out Of The Fact Chunk..
  if (0 != mmioAscend(m_hmmio,&ckOut1,0)) return E_FAIL;
       
  return S_OK;
} //EndWriteMMIOFunction

//////////////////////////////////////////////////////////////////////////////
// Construction/Destruction For TGRSound Class
//////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
// Name: TGRSound::TGRSound()
// Desc: Constructs The Class
//-----------------------------------------------------------------------------
TGRSound::TGRSound(LPDIRECTSOUNDBUFFER* apDSBuffer,DWORD dwDSBufferSize, 
DWORD dwNumBuffers,TGRWaveFile* pWaveFile,DWORD dwCreationFlags)
{
  DWORD I;
  m_apDSBuffer = new LPDIRECTSOUNDBUFFER[dwNumBuffers];
  if (NULL != m_apDSBuffer)
  {
    for (I = 0; I<dwNumBuffers; I++) m_apDSBuffer[I] = apDSBuffer[I];
    
    m_dwDSBufferSize = dwDSBufferSize;
    m_dwNumBuffers = dwNumBuffers;
    m_pWaveFile = pWaveFile;
    m_dwCreationFlags = dwCreationFlags;
        
    FillBufferWithSound( m_apDSBuffer[0], FALSE );
  }
} //EndConstructionDirectives

//-----------------------------------------------------------------------------
// Name: TGRSound::~TGRSound()
// Desc: Destroys The Class
//-----------------------------------------------------------------------------
TGRSound::~TGRSound()
{
  for( DWORD I = 0; I<m_dwNumBuffers; I++)
  {
    ReleaseClass(m_apDSBuffer[I]); 
  }

  DeleteArray(m_apDSBuffer); 
  DeleteClass(m_pWaveFile);
} //EndDestructionDirectives

//-----------------------------------------------------------------------------
// Name: TGRSound::Get3DBufferInterface()
// Desc: 
//-----------------------------------------------------------------------------
HRESULT TGRSound::Get3DBufferInterface(DWORD dwIndex,LPDIRECTSOUND3DBUFFER* ppDS3DBuffer)
{
  if (m_apDSBuffer == NULL) return CO_E_NOTINITIALIZED;
  if (dwIndex >= m_dwNumBuffers) return E_INVALIDARG;

  *ppDS3DBuffer = NULL;
  return m_apDSBuffer[dwIndex]->QueryInterface(IID_IDirectSound3DBuffer,(VOID**)ppDS3DBuffer);
} //EndGet3DBufferInterfaceFunction

//-----------------------------------------------------------------------------
// Name: TGRSound::FillBufferWithSound()
// Desc: Fills A DirectSound Buffer With A Sound File 
//-----------------------------------------------------------------------------
HRESULT TGRSound::FillBufferWithSound(LPDIRECTSOUNDBUFFER pDSB,BOOL bRepeatWavIfBufferLarger)
{
  HRESULT hr; 
  VOID* pDSLockedBuffer = NULL; //Pointer To Locked Buffer Memory
  DWORD dwDSLockedBufferSize = 0; //Size Of The Locked DirectSound Buffer
  DWORD dwWavDataRead = 0; //Amount Of Data Read From The Wav File 

  if (pDSB == NULL) return CO_E_NOTINITIALIZED;

  //Make Sure We Have Focus, And We Didn't Just Switch In From
  //An App Which Had A DirectSound Device
  if (FAILED(hr = RestoreBuffer(pDSB,NULL))) return hr;

  //Lock The Buffer Down
  if (FAILED(hr = pDSB->Lock(0,m_dwDSBufferSize,&pDSLockedBuffer, 
  &dwDSLockedBufferSize,NULL,NULL,0L))) return hr;

  //Reset The Wave File To The Beginning 
  m_pWaveFile->ResetFile();

  if (FAILED(hr = m_pWaveFile->Read((BYTE*)pDSLockedBuffer,
  dwDSLockedBufferSize,&dwWavDataRead))) return hr;

  if (dwWavDataRead == 0)
  {
    //Wav Is Blank, So Just Fill With Silence
    FillMemory((BYTE*)pDSLockedBuffer, 
    dwDSLockedBufferSize,(BYTE)(m_pWaveFile->m_pwfx->wBitsPerSample == 8 ? 128 : 0));
  }
  else 
	if( dwWavDataRead < dwDSLockedBufferSize )
    {
      //If The Wav File Was Smaller Than The DirectSound Buffer, 
      //We Need To Fill The Remainder Of The Buffer With Data 
      if (bRepeatWavIfBufferLarger)
      {       
        //Reset The File And Fill The Buffer With Wav Data
        DWORD dwReadSoFar = dwWavDataRead; //From Previous Call Above
        while (dwReadSoFar < dwDSLockedBufferSize)
        {  
          //This Will Keep Reading In Until The Buffer Is Full 
          //For Very Short Files
          if (FAILED(hr = m_pWaveFile->ResetFile())) return hr;

          hr = m_pWaveFile->Read((BYTE*)pDSLockedBuffer+dwReadSoFar,
          dwDSLockedBufferSize-dwReadSoFar,&dwWavDataRead);
          if (FAILED(hr)) return hr ;

          dwReadSoFar+= dwWavDataRead;
        } //End While
      }
      else
      {
        //Don't Repeat The Wav File, Just Fill In Silence 
        FillMemory((BYTE*)pDSLockedBuffer+dwWavDataRead, 
        dwDSLockedBufferSize-dwWavDataRead,(BYTE)(m_pWaveFile->m_pwfx->wBitsPerSample == 8 ? 128 : 0));
      } //End Else
   } //End If

  //Unlock The Buffer, We Don't Need It Anymore
  pDSB->Unlock(pDSLockedBuffer,dwDSLockedBufferSize,NULL,0);

  return S_OK;
} //EndFillBufferWithSoundFunction

//-----------------------------------------------------------------------------
// Name: TGRSound::GetFreeBuffer()
// Desc: Finding The First Buffer That Is Not Playing And Return A Pointer To 
//       It, Or If All Are Playing Return A Pointer To A Randomly Selected Buffer.
//-----------------------------------------------------------------------------
LPDIRECTSOUNDBUFFER TGRSound::GetFreeBuffer()
{
  DWORD I;
  if (m_apDSBuffer == NULL) return FALSE; 

  for (I = 0; I<m_dwNumBuffers; I++)
  {
    if (m_apDSBuffer[I])
    {  
      DWORD dwStatus = 0;
      m_apDSBuffer[I]->GetStatus(&dwStatus);
      if ((dwStatus & DSBSTATUS_PLAYING) == 0) break;
    }
  }

  if (I != m_dwNumBuffers) return m_apDSBuffer[I];
  else return m_apDSBuffer[rand() % m_dwNumBuffers];
} //EndGetFreeBufferFunction

//-----------------------------------------------------------------------------
// Name: TGRSound::GetBuffer()
// Desc: 
//-----------------------------------------------------------------------------
LPDIRECTSOUNDBUFFER TGRSound::GetBuffer(DWORD dwIndex)
{
  if (m_apDSBuffer == NULL) return NULL;
  if (dwIndex >= m_dwNumBuffers) return NULL;

  return m_apDSBuffer[dwIndex];
} //EndGetBufferFunction

//-----------------------------------------------------------------------------
// Name: TGRSound::RestoreBuffer()
// Desc: Restores The Lost Buffer. *pbWasRestored Returns TRUE If The Buffer Was 
//       Restored. It Can Also NULL If The Information Is Not Needed.
//-----------------------------------------------------------------------------
HRESULT TGRSound::RestoreBuffer(LPDIRECTSOUNDBUFFER pDSB,BOOL* pbWasRestored)
{
  HRESULT hr;
  if (pDSB == NULL) return CO_E_NOTINITIALIZED;
  if (pbWasRestored) *pbWasRestored = FALSE;

  DWORD dwStatus;
  if (FAILED(hr = pDSB->GetStatus(&dwStatus))) return hr;

  if (dwStatus & DSBSTATUS_BUFFERLOST)
  {
    //Since The App Could Have Just Been Activated, Then
    //DirectSound May Not Be Giving Us Control Yet, So 
    //The Restoring The Buffer May Fail.  
    //If It Does, Sleep Until DirectSound Gives Us Control.
    do 
    {
      hr = pDSB->Restore();
      if (hr == DSERR_BUFFERLOST) Sleep(10);
    }
    while ((hr = pDSB->Restore()) == DSERR_BUFFERLOST);

    if (pbWasRestored != NULL) *pbWasRestored = TRUE;
    return S_OK;
  }
  else
  {
    return S_FALSE;
  }
} //EndRestoreBufferFunction

//-----------------------------------------------------------------------------
// Name: TGRSound::Play()
// Desc: Plays The Sound Using Voice Management Flags. Pass In DSBPLAY_LOOPING
//       In The dwFlags To Loop The Sound
//-----------------------------------------------------------------------------
HRESULT TGRSound::Play(DWORD dwPriority,DWORD dwFlags,LONG lVolume,LONG lFrequency,LONG lPan)
{
  HRESULT hr;
  BOOL bRestored;
  if (m_apDSBuffer == NULL) return CO_E_NOTINITIALIZED;

  LPDIRECTSOUNDBUFFER pDSB = GetFreeBuffer();
  if (pDSB == NULL) return E_FAIL;

  //Restore The Buffer If It Was Lost
  if (FAILED(hr = RestoreBuffer(pDSB,&bRestored))) return hr;

  if (bRestored)
  {
    //The Buffer Was Restored, So We Need To Fill It With New Data
    if (FAILED(hr = FillBufferWithSound(pDSB,FALSE))) return hr;
  }

  if (m_dwCreationFlags & DSBCAPS_CTRLVOLUME)
  {
    pDSB->SetVolume(lVolume);
  }

  if (lFrequency != -1 && (m_dwCreationFlags & DSBCAPS_CTRLFREQUENCY))
  {
    pDSB->SetFrequency(lFrequency);
  }
    
  if (m_dwCreationFlags & DSBCAPS_CTRLPAN)
  {
    pDSB->SetPan(lPan);
  }
    
  return pDSB->Play(0,dwPriority,dwFlags);
} //EndTGRSoundPlayFunction

//-----------------------------------------------------------------------------
// Name: TGRSound::Play3D()
// Desc: Plays The Sound Using Voice Management Flags. Pass In DSBPLAY_LOOPING
//       In The dwFlags To Loop The Sound
//-----------------------------------------------------------------------------
HRESULT TGRSound::Play3D(LPDS3DBUFFER p3DBuffer,DWORD dwPriority,DWORD dwFlags,LONG lFrequency)
{
  HRESULT hr;
  BOOL bRestored;
  DWORD dwBaseFrequency;

  if (m_apDSBuffer == NULL) return CO_E_NOTINITIALIZED;

  LPDIRECTSOUNDBUFFER pDSB = GetFreeBuffer();
  if (pDSB == NULL) return E_FAIL;

  //Restore The Buffer If It Was Lost
  if (FAILED(hr = RestoreBuffer(pDSB,&bRestored))) return hr;

  if (bRestored)
  {
    //The Buffer Was Restored, So We Need To Fill It With New Data
    if (FAILED(hr = FillBufferWithSound(pDSB,FALSE))) return hr;
  }

  if (m_dwCreationFlags & DSBCAPS_CTRLFREQUENCY)
  {
    pDSB->GetFrequency(&dwBaseFrequency);
    pDSB->SetFrequency(dwBaseFrequency+lFrequency);
  }

  //QI For The 3D Buffer 
  LPDIRECTSOUND3DBUFFER pDS3DBuffer;
  hr = pDSB->QueryInterface(IID_IDirectSound3DBuffer,(VOID**)&pDS3DBuffer);
  if (SUCCEEDED(hr))
  {
    hr = pDS3DBuffer->SetAllParameters(p3DBuffer,DS3D_IMMEDIATE);
    if (SUCCEEDED(hr))
    {
      hr = pDSB->Play(0,dwPriority,dwFlags);
    }

    pDS3DBuffer->Release();
  }
  return hr;
} //EndTGRSoundPlay3DFunction

//-----------------------------------------------------------------------------
// Name: TGRSound::Stop()
// Desc: Stops The Sound From Playing
//-----------------------------------------------------------------------------
HRESULT TGRSound::Stop()
{
  if (m_apDSBuffer == NULL) return CO_E_NOTINITIALIZED;

  HRESULT hr = 0;

  for (DWORD I = 0; I<m_dwNumBuffers; I++) hr |= m_apDSBuffer[I]->Stop();
  return hr;
} //EndTGRSoundStopFunction

//-----------------------------------------------------------------------------
// Name: TGSound::Reset()
// Desc: Reset All Of The Sound Buffers
//-----------------------------------------------------------------------------
HRESULT TGRSound::Reset()
{
  if (m_apDSBuffer == NULL) return CO_E_NOTINITIALIZED;

  HRESULT hr = 0;

  for (DWORD I = 0; I<m_dwNumBuffers; I++)
  hr |= m_apDSBuffer[I]->SetCurrentPosition(0);

  return hr;
} //EndTGRSoundResetFunction

//-----------------------------------------------------------------------------
// Name: TGRSound::IsSoundPlaying()
// Desc: Checks To See If A Buffer Is Playing And Returns TRUE If It Is.
//-----------------------------------------------------------------------------
BOOL TGRSound::IsSoundPlaying()
{
  BOOL bIsPlaying = FALSE;

  if (m_apDSBuffer == NULL) return FALSE; 

  for (DWORD I = 0; I<m_dwNumBuffers; I++)
  {
    if (m_apDSBuffer[I])
    {  
      DWORD dwStatus = 0;
      m_apDSBuffer[I]->GetStatus(&dwStatus);
      bIsPlaying |= ((dwStatus & DSBSTATUS_PLAYING) != 0);
    }
  }
  return bIsPlaying;
} //EndIsSoundPlayingFunction

//////////////////////////////////////////////////////////////////////////////
// Construction/Destruction For TStreamingSound Class
//////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
// Name: TGRStreamingSound::TGRStreamingSound()
// Desc: Setups Up A Buffer So Data Can Be Streamed From The Wave File Into 
//       A Buffer. This Is Very Useful For Large Wav Files That Would Take A 
//       While To Load. The Buffer Is Initially Filled With Data, Then 
//       As Sound Is Played The Notification Events Are Signaled And More Data
//       Is Written Into The Buffer By Calling HandleWaveStreamNotification()
//-----------------------------------------------------------------------------
TGRStreamingSound::TGRStreamingSound(LPDIRECTSOUNDBUFFER pDSBuffer,DWORD dwDSBufferSize, 
TGRWaveFile* pWaveFile,DWORD dwNotifySize): TGRSound(&pDSBuffer,dwDSBufferSize,1,pWaveFile,0)           
{
  m_dwLastPlayPos = 0;
  m_dwPlayProgress = 0;
  m_dwNotifySize = dwNotifySize;
  m_dwNextWriteOffset = 0;
  m_bFillNextNotificationWithSilence = FALSE;
} //EndConstructionDirectives

//-----------------------------------------------------------------------------
// Name: TGRStreamingSound::~TGRStreamingSound()
// Desc: Destroys The Class
//-----------------------------------------------------------------------------
TGRStreamingSound::~TGRStreamingSound()
{
} //EndDestructionDirectives

//-----------------------------------------------------------------------------
// Name: TGRStreamingSound::HandleWaveStreamNotification()
// Desc: Handle The Notification That Tells Us To Put More Wav Data In The 
//       Circular Buffer
//-----------------------------------------------------------------------------
HRESULT TGRStreamingSound::HandleWaveStreamNotification(BOOL bLoopedPlay)
{
  HRESULT hr;
  DWORD dwCurrentPlayPos;
  DWORD dwPlayDelta;
  DWORD dwBytesWrittenToBuffer;
  VOID* pDSLockedBuffer = NULL;
  VOID* pDSLockedBuffer2 = NULL;
  DWORD dwDSLockedBufferSize;
  DWORD dwDSLockedBufferSize2;

  if (m_apDSBuffer == NULL||m_pWaveFile == NULL) return CO_E_NOTINITIALIZED;

  //Restore The Buffer If It Was Lost
  BOOL bRestored;
  if (FAILED(hr = RestoreBuffer(m_apDSBuffer[0],&bRestored))) return hr;

  if (bRestored)
  {
    //The Buffer Was Restored, So We Need To Fill It With New Data
    if (FAILED(hr = FillBufferWithSound(m_apDSBuffer[0],FALSE))) return hr;
    return S_OK;
  }

  //Lock The DirectSound Buffer
  if (FAILED(hr = m_apDSBuffer[0]->Lock(m_dwNextWriteOffset,m_dwNotifySize, 
  &pDSLockedBuffer,&dwDSLockedBufferSize,&pDSLockedBuffer2,&dwDSLockedBufferSize2,0L))) return hr;

  //m_dwDSBufferSize And m_dwNextWriteOffset Are Both Multiples Of m_dwNotifySize, 
  //It Should The Second Buffer, So It Should Never Be Valid
  if (pDSLockedBuffer2 != NULL)
  return E_UNEXPECTED; 

  if (!m_bFillNextNotificationWithSilence)
  {
    //Fill The DirectSound Buffer With Wav Data
    if (FAILED(hr = m_pWaveFile->Read((BYTE*)pDSLockedBuffer, 
    dwDSLockedBufferSize,&dwBytesWrittenToBuffer))) return hr;
  }
  else
  {
    //Fill The DirectSound Buffer With Silence
    FillMemory(pDSLockedBuffer,dwDSLockedBufferSize, 
    (BYTE)(m_pWaveFile->m_pwfx->wBitsPerSample == 8 ? 128 : 0));
    dwBytesWrittenToBuffer = dwDSLockedBufferSize;
  }

  //If The Number Of Bytes Written Is Less Than The 
  //Amount We Requested, We Have A Short File.
  if (dwBytesWrittenToBuffer < dwDSLockedBufferSize)
  {
    if (!bLoopedPlay) 
    {
      //Fill In Silence For The Rest Of The Buffer.
      FillMemory((BYTE*)pDSLockedBuffer+dwBytesWrittenToBuffer, 
      dwDSLockedBufferSize-dwBytesWrittenToBuffer, 
      (BYTE)(m_pWaveFile->m_pwfx->wBitsPerSample == 8 ? 128 : 0));

      //Any Future Notifications Should Just Fill The Buffer With Silence
      m_bFillNextNotificationWithSilence = TRUE;
    }
    else
    {
      //We Are Looping, So Reset The File And Fill The Buffer With Wav Data
      DWORD dwReadSoFar = dwBytesWrittenToBuffer; //From Previous Call Above
      while (dwReadSoFar < dwDSLockedBufferSize)
      {  
        //This Will Keep Reading In Until The Buffer Is Full (For Very Short Files)
        if (FAILED(hr = m_pWaveFile->ResetFile())) return hr;
        if (FAILED(hr = m_pWaveFile->Read((BYTE*)pDSLockedBuffer+dwReadSoFar,
        dwDSLockedBufferSize-dwReadSoFar,&dwBytesWrittenToBuffer))) return hr;

        dwReadSoFar+= dwBytesWrittenToBuffer;
      } //End While
    } //End Else
  } //End If

  //Unlock The DirectSound Buffer
  m_apDSBuffer[0]->Unlock(pDSLockedBuffer,dwDSLockedBufferSize,NULL,0);

  //Figure Out How Much Data Has Been Played So Far. When We Have Played
  //Past The End Of The File, We Will Either Need To Start Filling The
  //Buffer With Silence Or Starting Reading From The Beginning Of The File, 
  //Depending If The User Wants To Loop The Sound
  if (FAILED(hr = m_apDSBuffer[0]->GetCurrentPosition(&dwCurrentPlayPos,NULL))) return hr;

  //Check To See If The Position Counter Looped
  if (dwCurrentPlayPos < m_dwLastPlayPos)
  dwPlayDelta = (m_dwDSBufferSize-m_dwLastPlayPos)+dwCurrentPlayPos;
  else dwPlayDelta = dwCurrentPlayPos-m_dwLastPlayPos;

  m_dwPlayProgress+= dwPlayDelta;
  m_dwLastPlayPos = dwCurrentPlayPos;

  //If We Are Now Filling The Buffer With Silence, Then We Have Found The End So 
  //Check To See If The Entire Sound Has Played, If It Has Then Stop The Buffer
  if (m_bFillNextNotificationWithSilence)
  {
    //We Don't Want To Cut Off The Sound Before It's Done Playing
    if (m_dwPlayProgress >= m_pWaveFile->GetSize())
    {
      m_apDSBuffer[0]->Stop();
    }
  }

  //Update Where The Buffer Will Lock (For Next Time)
  m_dwNextWriteOffset+= dwDSLockedBufferSize; 
  m_dwNextWriteOffset%= m_dwDSBufferSize; //Circular Buffer

  return S_OK;
} //EndHandleWaveStreamNotificationFunction

//-----------------------------------------------------------------------------
// Name: TGRStreamingSound::Reset()
// Desc: Resets The Sound So It Will Begin Playing At The Beginning
//-----------------------------------------------------------------------------
HRESULT TGRStreamingSound::Reset()
{
  HRESULT hr;

  if (m_apDSBuffer[0] == NULL||m_pWaveFile == NULL) return CO_E_NOTINITIALIZED;

  m_dwLastPlayPos = 0;
  m_dwPlayProgress = 0;
  m_dwNextWriteOffset = 0;
  m_bFillNextNotificationWithSilence = FALSE;

  //Restore The Buffer If It Was Lost
  BOOL bRestored;
  if (FAILED(hr = RestoreBuffer(m_apDSBuffer[0],&bRestored))) return hr;

  if (bRestored)
  {
    //The Buffer Was Restored, So We Need To Fill It With New Data
    if (FAILED(hr = FillBufferWithSound(m_apDSBuffer[0],FALSE))) return hr;
  }

  m_pWaveFile->ResetFile();
  return m_apDSBuffer[0]->SetCurrentPosition(0L);  
} //EndTGRStreamingSoundResetFunction

//-----------------------------------------------------------------------------
