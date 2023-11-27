/*******************************************************************************
// It's Unofficial Version Of The GreenRay Engine v.3.0.0.2
// GR_Label.h: Interface For The CGRLabel Class.
// It's The GreenRay Label Realisation Method Engine.
// Made Specially For Implementing To Output Text Information On The Screen.
// Date Creation: 29 August 2005
// Last Changing: 5 August 2007
// Author Rights By: Zie Technology, Zombie Soft And Lucky's Flash Studio Inc.,
// Licensed By: (C)DDD Digital Dreams Development Inc., 2007
*******************************************************************************/
#include <GREngine.h>

////////////////////////////////////////////////////////////////////////////////
// Classes Used By This Header
////////////////////////////////////////////////////////////////////////////////
//class CGRLabel;

//Font Style Definitions
#define FS_NORMAL 0
#define FS_BOLD 1
#define FS_ITALIC 2
#define FS_BETA 3

//Font Color Definitions
#define clBlack 0
#define clWhite 1
#define clRed 2
#define clGreen 3
#define clBlue 4
#define clYellow 5

//-----------------------------------------------------------------------------
// Name: Struct CGRLabel
// Desc: Implementation Of The Text Out System Object
//-----------------------------------------------------------------------------
#if !defined(AFX_LABEL_H__)
#define AFX_LABEL_H__

class CGRLabel
{ 
public:
  /*Public Directives*/
  CGRLabel(LPDIRECT3DDEVICE9 pDevice);
  virtual ~CGRLabel();

  void Release();
  void Update();
  void Caption(TCHAR* Capt_String);
public:
  //Label Properties
  int FontSize;
  int FontColor;
  LPCSTR FontName;
  DWORD FontStyle;
  int Left;
  int Top;
private:
  /*Private Directives*/
  LPDIRECT3DDEVICE9 d_pDevice; //Pointer On Direct3D Device Interface
  ID3DXFont* m_pFont; //General Direct3D Font Interface
  //RGBA Palette Property 
  int Red;
  int Green;
  int Blue;
  int Alpha;
  void SetTextColor(int R,int G,int B,int A);
};
#endif //!defined(AFX_LABEL_H__)
