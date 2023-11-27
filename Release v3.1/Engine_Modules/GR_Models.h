/*******************************************************************************
// It's Unofficial Version Of The GreenRay Engine v.3.0.0.2
// GR_Models.h: Interface For The CGRModel Class.
// It's The GreenRay Model Loader Source File.
// Made Specially For Implementing Loading And Rendering Mesh Files System.
// Programming By: Verg
// Date Creation: 29 August 2005
// Last Changing: 24 August 2007
// Author Rights By: Zie Technology, Zombie Soft And Lucky's Flash Studio Inc.,
// Licensed By: (C)DDD Digital Dreams Development And (C)3D Studios Inc., 2007
*******************************************************************************/
#include <GREngine.h>

////////////////////////////////////////////////////////////////////////////////
// Classes Used By This Header
////////////////////////////////////////////////////////////////////////////////
//class CGRModel;

//-----------------------------------------------------------------------------
// Name: Struct CGRModel
// Desc: Class For Loading And Rendering File-Based Meshes
//-----------------------------------------------------------------------------
#if !defined(AFX_GRModel_H__)
#define AFX_GRModel_H__

class CGRModel
{
private:
  LPDIRECT3DDEVICE9 d_pDevice; //Pointer On Direct3D Device Interface
public:
  WCHAR m_strName[512];
  LPD3DXMESH m_pSysMemMesh; //SysMem Mesh, Lives Through Resize
  LPD3DXMESH m_pLocalMesh;  //Local Mesh, Rebuilt On Resize
    
  DWORD m_dwNumMaterials; //Materials For The Mesh
  D3DMATERIAL9* m_pMaterials;
  //IDirect3DBaseTexture9** m_pTextures;
  LPDIRECT3DTEXTURE9* m_pTextures; //Keep All Loadable Textures From Mesh Data
  LPCSTR m_pTexPath; //This Value Keep Texture Path
  bool m_bUseMaterials;
public:
  //Rendering The Mesh
  HRESULT Render(bool bDrawOpaqueSubsets = true,bool bDrawAlphaSubsets = true);
  HRESULT Render(ID3DXEffect* pEffect,
  D3DXHANDLE hTexture = NULL,D3DXHANDLE hDiffuse = NULL,
  D3DXHANDLE hAmbient = NULL,D3DXHANDLE hSpecular = NULL,
  D3DXHANDLE hEmissive = NULL,D3DXHANDLE hPower = NULL,
  bool bDrawOpaqueSubsets = true,bool bDrawAlphaSubsets = true);

  //Mesh Access
  LPD3DXMESH GetSysMemMesh() { return m_pSysMemMesh; }
  LPD3DXMESH GetLocalMesh() { return m_pLocalMesh; }

  //Rendering Options
  void UseMeshMaterials(bool bFlag) { m_bUseMaterials = bFlag; }
  HRESULT SetFVF(DWORD dwFVF);
  HRESULT SetVertexDecl(const D3DVERTEXELEMENT9* pDecl);

  //Initializing
  HRESULT RestoreObjects();
  HRESULT InvalidateObjects();
    
  //Set Texture Disposition Folder
  void SetTexturePath(LPCSTR Tex_Path);

  //Creation/Destruction
  HRESULT Create(LPDIRECT3DDEVICE9 pDevice,LPCWSTR File_Name);
  HRESULT CreateMaterials(LPCWSTR Str_Path, 
  ID3DXBuffer* pAdjacencyBuffer,ID3DXBuffer* pMtrlBuffer);
    
  CGRModel();
  virtual ~CGRModel();
  HRESULT Destroy();
};
#endif //!defined(AFX_GRModel_H__)