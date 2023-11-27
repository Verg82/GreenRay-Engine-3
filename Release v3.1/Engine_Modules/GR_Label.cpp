/*******************************************************************************
// It's Unofficial Version Of The GreenRay Engine v.3.0.0.2
// GR_Label.cpp
// It's The GreenRay Label Realisation Method Engine.
// Made Specially For Implementing To Output Text Information On The Screen.
// Date Creation: 29 August 2005
// Last Changing: 5 August 2007
// Author Rights By: Zie Technology, Zombie Soft And Lucky's Flash Studio Inc.,
// Licensed By: (C)DDD Digital Dreams Development Inc., 2007
*******************************************************************************/
#include <GR_Label.h>
#include <GRMethods.h>

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction For CGRLabel Class
///////////////////////////////////////////////////////////////////////////////
CGRLabel::CGRLabel(LPDIRECT3DDEVICE9 pDevice)
{
  d_pDevice = pDevice; //Apropritate Device Pointer To Class
  m_pFont = NULL;

  //Set Default Font Parameters
  FontColor = clBlack; //Set Font Color
  FontName = "MS Sans Serif"; //Set Font Name
  FontSize = 8; //Set Font Size
  FontStyle = FS_NORMAL; //Set Font Style
  Left = 0;
  Top = 0; 

} //EndConstructionDirectives

CGRLabel::~CGRLabel()
{ 
} //EndDestructionDirectives

//-----------------------------------------------------------------------------
// Name: CGRLabel::Release()
// Desc: 
//-----------------------------------------------------------------------------
void CGRLabel::Release()
{
  ReleaseClass(m_pFont);
} //EndReleaseProcedure

//-----------------------------------------------------------------------------
// Name: CGRLabel::UpdateParam()
// Desc: 
//-----------------------------------------------------------------------------
void CGRLabel::Update()
{
  //If Founded Early Created Font Then Release Him And Prepare New 
  if (m_pFont != NULL) { ReleaseClass(m_pFont); }

  HDC DC = GetDC(0);
  int nLogPixelsY = GetDeviceCaps(DC,LOGPIXELSY);
  ReleaseDC(0,DC);
 
  int Cap_Size = -FontSize*nLogPixelsY/72; //Set Font Size
 
  //Setup Font Color
  if (FontColor == 0) SetTextColor(255,0,0,0); //Set Text Color To Black 
  if (FontColor == 1) SetTextColor(255,255,255,233); //Set Text Color To White 
  if (FontColor == 2) SetTextColor(255,255,0,0); //Set Text Color To Red 
  if (FontColor == 3) SetTextColor(255,0,255,0); //Set Text Color To Green
  if (FontColor == 4) SetTextColor(255,103,165,236); //Set Text Color To Blue
  if (FontColor == 5) SetTextColor(255,255,255,50); //Set Text Color To Yellow 

  //Create Direct3D Font
  if (FAILED(D3DXCreateFont(d_pDevice,Cap_Size,0,FontStyle,0,FALSE,DEFAULT_CHARSET,
  OUT_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_DONTCARE,FontName,&m_pFont))) 
  {
	ShowMessage("GR_Label.cpp: Can't Create D3DXFont");
    return;
  }
} //EndUpdateParamProcedure

//-----------------------------------------------------------------------------
// Name: CGRLabel::SetTextColor()
// Desc: 
//-----------------------------------------------------------------------------
void CGRLabel::SetTextColor(int R,int G,int B,int A)
{
  Red = R; Green = G; Blue = B; Alpha = A; //Set Text Color
} //EndSetTextColorProcedure

//-----------------------------------------------------------------------------
// Name: CGRLabel::Caption()
// Desc: Draw And Display Direct3D Text From Label
//-----------------------------------------------------------------------------
void CGRLabel::Caption(TCHAR* Capt_String)
{
  RECT Out_Rect;
  
  //Set Text Position
  Out_Rect.left = Left;
  Out_Rect.top = Top;
  Out_Rect.right = 0;
  Out_Rect.bottom = 0;

  //Output Text
  m_pFont->DrawText(NULL,Capt_String,-1,&Out_Rect,DT_NOCLIP,D3DCOLOR_ARGB(Red,Green,Blue,Alpha));
} //EndCaptionProcedure

//-----------------------------------------------------------------------------