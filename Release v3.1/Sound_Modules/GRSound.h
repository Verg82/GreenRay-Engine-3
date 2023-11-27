/*******************************************************************************
// It's Unofficial Version Of The GreenRay Engine v.3.0.0.2
// GRSound.h: 
// DirectSound Framework Classes For Reading And Writing Wav Files And
// Playing Them In DirectSound Buffers. Feel Free To Use This Class 
// As A Starting Point For Adding Extra Functionality.
// Date Creation: 29 August 2005
// Last Changing: 13 March 2006
// Author Rights By: Zie Technology, Zombie Soft And Lucky's Flash Studio Inc.,
// Licensed By: (C)DDD Digital Dreams Development Inc., 2006
*******************************************************************************/
//#include <mmsystem.h>
//#include <dmusici.h>
#include <GREngine.h>
#include <GRMethods.h>

////////////////////////////////////////////////////////////////////////////////
// Classes Used By This Header
////////////////////////////////////////////////////////////////////////////////
class TGRWave;
class CGRMusic;
class TGRSoundManager;
class TGRWaveFile;
class TGRSound;
class TGRStreamingSound;

////////////////////////////////////////////////////////////////////////////////
// Global GRSound Constants 
////////////////////////////////////////////////////////////////////////////////
#define WAVEFILE_READ 1
#define WAVEFILE_WRITE 2

//-----------------------------------------------------------------------------
// Name: Class TGRWave
// Desc: Base Class Of The GreenRay Wave Interface For Initialize 
// DirectSound Functions.
//-----------------------------------------------------------------------------
#if !defined(AFX_GRWave_H__)
#define AFX_GRWave_H__
class TGRWave
{
private:
	/*Private Directives*/
	TGRSoundManager* gr_SoundManager; //GreenRay DirectSound Manager Class
    TGRSound* gr_Sound; //The GreenRay Sound Base Interface
public:
	/*Public Directives*/
	HRESULT CreateSoundInterface(HWND _HWnd,TCHAR* FileName,int Channels,int Frequency, int Bitrate);
    void PlaybackSound();
	void PlaybackReset();
	void PlaybackLooping();
	void StopSound();
	void ReleaseSound();

	TGRWave();
    virtual ~TGRWave();
protected:
   /*Protected Directives*/
}; 
#endif //!defined(AFX_GRWave_H__)

//-----------------------------------------------------------------------------
// Name: Class CGRMusic
// Desc: Base Class Of The GreenRay Music Mp3 Interface For Initialize 
// DirectSound Functions.
//-----------------------------------------------------------------------------
#if !defined(AFX_GRMusic_H__)
#define AFX_GRMusic_H__
class CGRMusic
{
private:
  /*Private Directives*/
  //IBaseFilter* pBaseFilter;
  IGraphBuilder* pGraphBuilder;
  IMediaControl* pMediaControl;
  IMediaEventEx* pMediaEventEx;
public:
  /*Public Directives*/
  BOOL bUnplay;

  CGRMusic();
  ~CGRMusic();  
  void Release(); 

  HRESULT CreateSound(LPCWSTR FileName);
  void PlaySound();
  void PauseSound();
  void StopSound();
  BOOL IsUnplaying();
protected:
  /*Protected Directives*/
};
#endif //!defined(AFX_GRMusic_H__)

//-----------------------------------------------------------------------------
// Name: Class TGRSoundManager
// Desc: Initialize The GreenRay Wave Interface For Playback Of Wave Files.
//-----------------------------------------------------------------------------
#if !defined(AFX_GRSoundManager_H__)
#define AFX_GRSoundManager_H__
class TGRSoundManager
{
private:
	/*Private Directives*/
public:
	/*Public Directives*/
	TGRSoundManager();
    virtual ~TGRSoundManager();
    
    //inline LPDIRECTSOUND8 GetDirectSound() { return m_pDS; }
	HRESULT Initialize(HWND _HWnd,DWORD dwCoopLevel);
	HRESULT SetPrimaryBufferFormat(DWORD dwPrimaryChannels,DWORD dwPrimaryFreq,DWORD dwPrimaryBitRate);
	HRESULT Create(TGRSound** ppSound,LPTSTR strWaveFileName, 
	DWORD dwCreationFlags = 0,GUID _GUID3DAlgorithm = GUID_NULL,DWORD dwNumBuffers = 1);
	HRESULT CreateFromMemory(TGRSound** ppSound,BYTE* pbData,ULONG ulDataSize, 
	LPWAVEFORMATEX pwfx,DWORD dwCreationFlags = 0,GUID _GUID3DAlgorithm = GUID_NULL,DWORD dwNumBuffers = 1);
	HRESULT CreateStreaming(TGRStreamingSound** ppStreamingSound,LPTSTR strWaveFileName, 
	DWORD dwCreationFlags,GUID _GUID3DAlgorithm,DWORD dwNotifyCount,DWORD dwNotifySize,HANDLE hNotifyEvent);
	HRESULT Get3DListenerInterface(LPDIRECTSOUND3DLISTENER* ppDSListener);
protected:
   /*Protected Directives*/ 
   IDirectSound8* m_pDirectSound; //Base DirectSound Interface
}; 
#endif //!defined(AFX_GRSoundManager_H__)

//-----------------------------------------------------------------------------
// Name: Class TGRWaveFile
// Desc: Encapsulates Reading Or Writing Sound Data To Or From A Wave Files
//-----------------------------------------------------------------------------
#if !defined(AFX_GRWaveFile_H__)
#define AFX_GRWaveFile_H__
class TGRWaveFile
{
private:
	/*Private Directives*/
public:
	/*Public Directives*/
	WAVEFORMATEX* m_pwfx;        // Pointer To WAVEFORMATEX Structure
    HMMIO         m_hmmio;       // MM I/O Handle For The WAVE
    MMCKINFO      m_ck;          // Multimedia RIFF Chunk
    MMCKINFO      m_ckRiff;      // Use In Opening A WAVE File
    DWORD         m_dwSize;      // The Size Of The wave File
    MMIOINFO      m_mmioinfoOut;
    DWORD         m_dwFlags;
    BOOL          m_bIsReadingFromMemory;
    BYTE*         m_pbData;
    BYTE*         m_pbDataCur;
    ULONG         m_ulDataSize;
    CHAR*         m_pResourceBuffer;

	TGRWaveFile();
    ~TGRWaveFile();

	HRESULT Open(LPTSTR strFileName,WAVEFORMATEX* pwfx,DWORD dwFlags);
	HRESULT OpenFromMemory(BYTE* pbData,ULONG ulDataSize,WAVEFORMATEX* pwfx,DWORD dwFlags);
	HRESULT ResetFile();
	DWORD GetSize();
	HRESULT Read(BYTE* pBuffer,DWORD dwSizeToRead,DWORD* pdwSizeRead);
    HRESULT Write(UINT nSizeToWrite,BYTE* pbData,UINT* pnSizeWrote);
    HRESULT Close();

    WAVEFORMATEX* GetFormat() { return m_pwfx; };
protected:
   /*Protected Directives*/ 
   HRESULT ReadMMIO();
   HRESULT WriteMMIO(WAVEFORMATEX *pwfxDest);
}; 
#endif //!defined(AFX_GRWaveFile_H__)

//-----------------------------------------------------------------------------
// Name: Class TGRSound
// Desc: Encapsulates Functionality Of A DirectSound Buffer.
//-----------------------------------------------------------------------------
#if !defined(AFX_GRSound_H__)
#define AFX_GRSound_H__
class TGRSound
{
private:
	/*Private Directives*/
public:
	/*Public Directives*/
	TGRSound(LPDIRECTSOUNDBUFFER* apDSBuffer,DWORD dwDSBufferSize,
    DWORD dwNumBuffers,TGRWaveFile* pWaveFile,DWORD dwCreationFlags);
    virtual ~TGRSound();
  
	HRESULT Get3DBufferInterface(DWORD dwIndex,LPDIRECTSOUND3DBUFFER* ppDS3DBuffer);
	HRESULT FillBufferWithSound(LPDIRECTSOUNDBUFFER pDSB,BOOL bRepeatWavIfBufferLarger);
	LPDIRECTSOUNDBUFFER GetFreeBuffer();
    LPDIRECTSOUNDBUFFER GetBuffer(DWORD dwIndex);

    HRESULT Play(DWORD dwPriority = 0,DWORD dwFlags = 0,LONG lVolume = 0,LONG lFrequency = -1,LONG lPan = 0);
    HRESULT Play3D(LPDS3DBUFFER p3DBuffer,DWORD dwPriority = 0,DWORD dwFlags = 0,LONG lFrequency = 0);
    HRESULT Stop();
    HRESULT Reset();
    BOOL IsSoundPlaying();
protected:
    /*Protected Directives*/ 
	LPDIRECTSOUNDBUFFER* m_apDSBuffer;
    DWORD                m_dwDSBufferSize;
    TGRWaveFile*         m_pWaveFile;
    DWORD                m_dwNumBuffers;
    DWORD                m_dwCreationFlags;

	HRESULT RestoreBuffer(LPDIRECTSOUNDBUFFER pDSB,BOOL* pbWasRestored);
}; 
#endif //!defined(AFX_GRSound_H__)

//-----------------------------------------------------------------------------
// Name: Class TGRStreamingSound
// Desc: Encapsulates Functionality To Play A Wave File With DirectSound.  
//       The Create() Method Loads A Chunk Of Wave File Into The Buffer, 
//       And As Sound Plays More Is Written To The Buffer By Calling 
//       HandleWaveStreamNotification() Whenever hNotifyEvent Is Signaled.
//-----------------------------------------------------------------------------
#if !defined(AFX_GRStreamingSound_H__)
#define AFX_GRStreamingSound_H__
class TGRStreamingSound: public TGRSound
{
private:
	/*Private Directives*/
public:
	/*Public Directives*/
    TGRStreamingSound(LPDIRECTSOUNDBUFFER pDSBuffer,DWORD dwDSBufferSize, 
	TGRWaveFile* pWaveFile,DWORD dwNotifySize);
    ~TGRStreamingSound();

    HRESULT HandleWaveStreamNotification(BOOL bLoopedPlay);
    HRESULT Reset();
protected:
	/*Protected Directives*/
    DWORD m_dwLastPlayPos;
    DWORD m_dwPlayProgress;
    DWORD m_dwNotifySize;
    DWORD m_dwNextWriteOffset;
    BOOL m_bFillNextNotificationWithSilence;
};
#endif //!defined(AFX_GRStreamingSound_H__)