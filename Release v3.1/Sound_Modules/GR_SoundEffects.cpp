/*******************************************************************************
// It's Unofficial Version Of The GreenRay Engine v.3.0.0.2
// GR_SoundEffects.cpp
// It's The GreenRay Sound Effects Engine Source File.
// Made Specially For Implementing Ambient Music And Sound Effects.
// Date Creation: 29 August 2005
// Last Changing: 31 March 2007
// Licensed By: (C)DDD Digital Dreams Development And (C)3D Studios Inc., 2007
*******************************************************************************/
#include <GR_SoundEffects.h>

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction For CGRSoundEffects Class
///////////////////////////////////////////////////////////////////////////////
CGRSoundEffects::CGRSoundEffects()
{
  //d_pDevice = pDevice; //Apropritate Device Pointer To Class

  //Create FootStep Sound
  //m_pFootStep = new TGRWave();
  //m_pFootStep->CreateSoundInterface(_HWnd,TEXT("\Sounds\\Player\\Foot\\Stone\\Tile_01.wav"),2,44100,32);

  //Create Tube Down Sound
  m_pSndTube = new TGRWave();
  m_pSndTube->CreateSoundInterface(FormL_HWnd,TEXT("\Sounds\\Weapon\\Beretta\\TubeDownOne.wav"),2,44100,32);
  
  //Create Richoshet Sound
  m_pSndRichoshet = new TGRWave();
  m_pSndRichoshet->CreateSoundInterface(FormL_HWnd,TEXT("\Sounds\\Weapon\\Beretta\\Richoshet.wav"),2,44100,32);
  

} //EndConstructionDirectives

CGRSoundEffects::~CGRSoundEffects()
{ 
} //EndDestructionDirectives

//-----------------------------------------------------------------------------
// Name: CGRSoundEffects::Release()
// Desc: 
//-----------------------------------------------------------------------------
void CGRSoundEffects::Release()
{
  //Free Tube Down Sound
  if (m_pSndTube != NULL) { m_pSndTube->ReleaseSound(); m_pSndTube = NULL; }
  //Free Richoshet Sound
  if (m_pSndRichoshet != NULL) { m_pSndRichoshet->ReleaseSound(); m_pSndRichoshet = NULL; }

} //EndReleaseProcedure

//-----------------------------------------------------------------------------
// Name: CGRSoundEffects::PlaybackSound()
// Desc: 
//-----------------------------------------------------------------------------
void CGRSoundEffects::PlaybackSound(LPCSTR Param)
{
  //Playing Tube Down Sound
  if (Param == "TubeDown") { if (m_pSndTube != NULL) m_pSndTube->PlaybackSound(); } 
  //Playing Richoshet Sound
  if (Param == "Richoshet") { if (m_pSndRichoshet != NULL) m_pSndRichoshet->PlaybackSound(); } 

} //EndPlaybackSoundProcedure

//-----------------------------------------------------------------------------
// Name: CGRSoundEffects::PlaybackReset()
// Desc: 
//-----------------------------------------------------------------------------
void CGRSoundEffects::PlaybackReset(LPCSTR Param)
{
  //Playing Tube Down Sound With Reset
  if (Param == "TubeDown") { if (m_pSndTube != NULL) m_pSndTube->PlaybackReset(); } 
  //Playing Richoshet Sound With Reset
  if (Param == "Richoshet") { if (m_pSndRichoshet != NULL) m_pSndRichoshet->PlaybackReset(); } 
} //EndPlaybackResetProcedure

//-----------------------------------------------------------------------------
// Name: CGRSoundEffects::GetSound()
// Desc: 
//-----------------------------------------------------------------------------
TGRWave* CGRSoundEffects::GetSound(LPCSTR Param)
{
  //Get Tube Down Sound Class From Container
  if (Param == "TubeDown") { if (m_pSndTube != NULL) return m_pSndTube; } 
  //Get Richoshet Sound Class From Container
  if (Param == "Richoshet") { if (m_pSndRichoshet != NULL) return m_pSndRichoshet; } 
 
} //EndGetSoundFunction