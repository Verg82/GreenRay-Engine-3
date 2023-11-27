/*******************************************************************************
// It's Unofficial Version Of The GreenRay Engine v.3.0.0.1
// GRShaders.h: Interface For The CGRShaders Class.
// It's The GreenRay Shader Engine.
// Made Specially For Implementing Shader Effects.
// Date Creation: 29 August 2005
// Last Changing: 17 August 2006
// Author Rights By: Zie Technology, Zombie Soft And Lucky's Flash Studio Inc.,
// Licensed By: (C)DDD Digital Dreams Development And (C)3D Studios Inc., 2006
*******************************************************************************/
#include <GREngine.h>

////////////////////////////////////////////////////////////////////////////////
// Exporting Procedures And Functions
////////////////////////////////////////////////////////////////////////////////
//HRESULT LoadShaderFile(LPDIRECT3DDEVICE9 pd3dDevice,
//LPWSTR FileName,IDirect3DVertexShader9** shader,IDirect3DVertexDeclaration9** declaration,D3DVERTEXELEMENT9 *decl);
//HRESULT LoadShaderFile(LPDIRECT3DDEVICE9 pd3dDevice,
//LPWSTR FileName,IDirect3DPixelShader9** shader);

HRESULT CreateVertDeclaration(LPDIRECT3DDEVICE9 pDevice,IDirect3DVertexDeclaration9** Declaration,D3DVERTEXELEMENT9* Decl);
HRESULT LoadShaderFile(LPDIRECT3DDEVICE9 pDevice,LPCSTR FileName,LPD3DXEFFECT* Effect);

////////////////////////////////////////////////////////////////////////////////
// Classes Used By This Header
////////////////////////////////////////////////////////////////////////////////
//class CGRShaders;

//-----------------------------------------------------------------------------
// Name: Struct CGRShaders
// Desc:
//-----------------------------------------------------------------------------
#if !defined(AFX_SHADERS_H__)
#define AFX_SHADERS_H__

class CGRShaders
{
public:
  /*Public Directives*/
  CGRShaders();
  virtual ~CGRShaders();
  
private:
  /*Private Directives*/

};
#endif //!defined(AFX_SHADERS_H__)