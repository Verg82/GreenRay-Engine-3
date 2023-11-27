/*******************************************************************************
// It's Unofficial Version Of The GreenRay Engine v.3.0.0.1
// GRInterface.h: Interface For The CGRInterface Class.
// It's The GreenRay User Interface Engine.
// Made Specially For Implementing User Interface.
// Date Creation: 29 August 2005
// Last Changing: 17 August 2006
// Author Rights By: Zie Technology, Zombie Soft And Lucky's Flash Studio Inc.,
// Licensed By: (C)DDD Digital Dreams Development And (C)3D Studios Inc., 2006
*******************************************************************************/
#include <GREngine.h>

////////////////////////////////////////////////////////////////////////////////
// Classes Used By This Header
////////////////////////////////////////////////////////////////////////////////
//class CGRInterface;

//-----------------------------------------------------------------------------
// Name: Struct CGRInterface
// Desc:
//-----------------------------------------------------------------------------
#if !defined(AFX_INTERFACE_H__)
#define AFX_INTERFACE_H__

//Define A FVF For Our Interface Panel
#define PANEL_D3DFVF_VERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1)

class CGRInterface
{ 
public:
  /*Public Directives*/
  
  //Define A Custom Vertex For Our UI Panel
  struct PANEL_VERTEX
  {
    FLOAT x,y,z; //Position Of Vertex In Space
	FLOAT tu,tv; //Texture Coordinates
  };

  CGRInterface(LPDIRECT3DDEVICE9 pDevice,int Top,int Left,int Width,int Height,int Opacity);
  virtual ~CGRInterface();
  void Release();
  void CreateTexture(TCHAR* FileName,DWORD dwKeyColor);
  bool GetInplace(int x,int y); 
  void RenderUIOverlay();
private:
  /*Private Directives*/
  LPDIRECT3DDEVICE9 d_pDevice; //Pointer On Direct3D Device Interface
  LPDIRECT3DVERTEXDECLARATION9 m_pUIVertexDecl; //Vertex Declaration Interface
  LPD3DXEFFECT m_pUIEffect; //Effect For Visualize User Interface 
  D3DXHANDLE m_pUITechHandle; //Effect Technique Handle
  LPDIRECT3DVERTEXBUFFER9 m_pUIVertexBuffer; //Vertex Buffer For UI 
  LPDIRECT3DTEXTURE9 m_pUITexture; //Texture UI Interface

  float m_fOpacity; //Set Opacity Level
  int m_nWidth; //Width Of Interface Panel
  int m_nHeight; //Height Of Interface Panel
  int m_nTop; //Top Position Of Interface Panel
  int m_nLeft; //Left Position Of Interface Panel

  HRESULT CreateVertexBuffer(); //It's For Create UI Panel Vertex Buffer
};
#endif //!defined(AFX_INTERFACE_H__)
