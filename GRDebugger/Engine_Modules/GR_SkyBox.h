/*******************************************************************************
// It's Unofficial Version Of The GreenRay Engine v.3.0.0.2
// GR_SkyBox.h: Interface For The SkyBox Geometry Implementation Class.
// It's The GreenRay SkyBox Geometry Transform Engine.
// Made Specially For Implementing Only SkyBox Geometry And Effects.
// Programming By: Verg
// Date Creation: 29 August 2005
// Last Changing: 03 July 2009
// Author Rights By: Zie Technology And Lucky's Flash Studio Inc.,
// Licensed By: (C)DDD Digital Dreams Development And (C)3D Studios Inc., 2009
*******************************************************************************/
#include <GR_Engine.h>
#include <GR_Methods.h>
#include <GR_Camera.h>

////////////////////////////////////////////////////////////////////////////////
// Classes Used By This Header
////////////////////////////////////////////////////////////////////////////////
//class CGRSkyBox;

//-----------------------------------------------------------------------------
// Name: CGRSkyBox
// Desc: The Box Class
//-----------------------------------------------------------------------------
#if !defined(AFX_SKYBOX_H__)
#define AFX_SKYBOX_H__

// онстанта FVF содержаща€ формат и структуру вершин 
#define D3DFVF_SKYBOXVERTEX (D3DFVF_XYZ|D3DFVF_TEX1|D3DFVF_TEXCOORDSIZE3(0)) //Vertex Format

class CGRSkyBox
{
public:
   /*Public Directives*/
  //SkyBox Vertex, Normals, Texures Positions
  //—труктура содержаща€ координаты вершин, нормали,
  //цвет и текстурные координаты.
  struct SKYBOX_VERTEX
  {
    FLOAT x,y,z;    //Coordinates
    FLOAT nx,ny,nz; //Normals
    DWORD _Color;   //Vertex Color
    FLOAT tu,tv;    //Textures
  }; //End SkyBox Vertex Formats

  CGRSkyBox(LPDIRECT3DDEVICE9 pDevice);
  virtual ~CGRSkyBox();
  void Release();
  void SetTextures(TCHAR* _TexBack,TCHAR* _TexLeft,TCHAR* _TexFront,
  TCHAR* _TexRight,TCHAR* _TexFloor,TCHAR* _TexCeiling);
  HRESULT CreateBox(float fWidth,float fHeight,float fDepth);
  void Render();
protected:
  /*Protected Directives*/
  LPDIRECT3DDEVICE9 d_pDevice; //Pointer On Direct3D Device Interface
  IDirect3DVertexBuffer9* m_pVertexBuffer; //The Vertex Buffer
  IDirect3DIndexBuffer9* m_pIndexBuffer; //The Index Buffer
  IDirect3DTexture9* m_pTex[6]; //Variable Keep Same Textures
  TCHAR* m_TexPath[6]; //Variable Keep Texture Path
  float m_pWidth,m_pHeight,m_pDepth; //Variables Keep SkyBox WHD Size
};
#endif //!defined(AFX_SKYBOX_H__)