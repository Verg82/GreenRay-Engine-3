/*******************************************************************************
// It's Unofficial Version Of The GreenRay Engine v.3.0.0.2
// GR_SoundEffects.h: Interface For The CGRSoundEffects Class.
// It's The GreenRay Sound Effects Engine.
// Made Specially For Implementing Ambient Music And Sound Effects.
// Date Creation: 29 August 2005
// Last Changing: 31 March 2007
// Licensed By: (C)DDD Digital Dreams Development And (C)3D Studios Inc., 2007
*******************************************************************************/
#include <GREngine.h>
#include <GRMethods.h>
#include <GRSound.h>

////////////////////////////////////////////////////////////////////////////////
// Classes Used By This Header
////////////////////////////////////////////////////////////////////////////////
//class CGRSoundEffects;

//-----------------------------------------------------------------------------
// Name: Struct CGRSoundEffects
// Desc: 
//-----------------------------------------------------------------------------
#if !defined(AFX_GRSoundEffects_H__)
#define AFX_GRSoundEffects_H__
class CGRSoundEffects
{	
public:
  /*Public Directives*/
  CGRSoundEffects();
  virtual ~CGRSoundEffects();

  void Release();
  void PlaybackSound(LPCSTR Param);
  void PlaybackReset(LPCSTR Param);
  TGRWave* GetSound(LPCSTR Param);
public:  
  /*Public Directives*/
private:
  /*Private Directives*/
  //LPDIRECT3DDEVICE9 d_pDevice; //Pointer On Direct3D Device Interface
  //TGRWave* m_pFootStep; //Wave Directive For Playing FootSteps Sound
  TGRWave* m_pSndTube; //Wave Directive For Tube Down Sound
  TGRWave* m_pSndRichoshet; //Wave Directive For Richoshet Sound
protected:
  /*Protected Directives*/
};
#endif //!defined(AFX_GRSoundEffects_H__)

extern CGRSoundEffects* gr_SoundsEff; //

