/*******************************************************************************
// It's Unofficial Version Of The GreenRay Engine v.3.0.0.3
// GR_World.h: Interface For The WorldMap Geometry Implementation Class.
// It's The GreenRay WorldMap Geometry Transform Engine.
// Made Specially For Implementing Only WorldMap Geometry Effects.
// Programming By: Verg
// Date Creation: 29 August 2005
// Last Changing: 26 August 2007
// Author Rights By: Zie Technology And Lucky's Flash Studio Inc.,
// Licensed By: (C)DDD Digital Dreams Development And (C)3D Studios Inc., 2007
*******************************************************************************/
#include <GREngine.h>
#include <GRMethods.h>
#include <GR_Camera.h>

////////////////////////////////////////////////////////////////////////////////
// Classes Used By This Header
////////////////////////////////////////////////////////////////////////////////
//class CGRSkyBox;
//class CGRMap;

//-----------------------------------------------------------------------------
// Name: CGRSkyBox
// Desc: The Box Class
//-----------------------------------------------------------------------------
#if !defined(AFX_SKYBOX_H__)
#define AFX_SKYBOX_H__

#define D3DFVF_SKYBOXVERTEX (D3DFVF_XYZ|D3DFVF_TEX1|D3DFVF_TEXCOORDSIZE3(0)) //Vertex Format

class CGRSkyBox
{
public:
   /*Public Directives*/
  //Box Vertex, Normals, Texures Positions
  struct SKYBOX_VERTEX
  {
    FLOAT x,y,z;    //Coordinates
    FLOAT nx,ny,nz; //Normals
    DWORD _Color;   //Vertex Color
    FLOAT tu,tv;    //Textures
  }; //End Box Vertex Formats

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
  IDirect3DTexture9* m_pTex[6]; //Variable To Hold Same Textures
  TCHAR* m_TexPath[6]; //Variable To Hold Texture Path
  float m_pWidth,m_pHeight,m_pDepth; //Variables Keept SkyBox WHD Size
};
#endif //!defined(AFX_SKYBOX_H__)

//-----------------------------------------------------------------------------
// Name: CGRMap
// Desc: The Map Environment Class
//-----------------------------------------------------------------------------
#if !defined(AFX_MAP_H__)
#define AFX_MAP_H__

class CGRMap
{
public:
   /*Public Directives*/
  CGRMap();
  virtual ~CGRMap();

  void Create(LPDIRECT3DDEVICE9 pDevice,LPCWSTR File_Name);
  void Release();
  void SetTexturePath(LPCSTR Tex_Path);
  void Render();
  LPD3DXMESH GetLocalMeshGeometry();
  CGRModel* GetCGRModel();
protected:
  /*Protected Directives*/
  LPDIRECT3DDEVICE9 d_pDevice; //Pointer On Direct3D Device Interface
  LPDIRECT3DVERTEXDECLARATION9 pVertDecl; // Vertex Declaration For Loadable Model
  LPCSTR m_pTexPath; //This Value Keep Texture Path

  CGRModel* m_pMap; //Map Model Class
  ID3DXEffect* g_pEffect; //D3DX Effect Interface
  D3DXMATERIAL* g_pMaterials; //Pointer To Material Info In m_pbufMaterials

  IDirect3DTexture9* LightTexture;
};
#endif //!defined(AFX_MAP_H__)