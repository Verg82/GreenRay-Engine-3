/*******************************************************************************
// It's Unofficial Version Of The GreenRay Engine v.1.0.0.3
// GR_Weapon.h
// It's The GreenRay Weapon Class Header File.
// Date Creation: 29 August 2005
// Last Changing: 23 February 2007
// Author Rights By: Zie Technology, Zombie Soft And Lucky's Flash Studio Inc.,
// Licensed By: (C)DDD Digital Dreams Development Inc., 2007
*******************************************************************************/
#include <GREngine.h>
#include <GRMethods.h>
#include <GRModels.h>
#include <GR_Camera.h>

////////////////////////////////////////////////////////////////////////////////
// Classes Used By This Header
////////////////////////////////////////////////////////////////////////////////
//class CGRWeapon;

//-----------------------------------------------------------------------------
// Name: Struct CGRWeapon
// Desc: 
//-----------------------------------------------------------------------------
#if !defined(AFX_GRWeapon_H__)
#define AFX_GRWeapon_H__
class CGRWeapon
{	
public:
  /*Public Directives*/
  CGRWeapon(LPDIRECT3DDEVICE9 pDevice);
  virtual ~CGRWeapon();

  void Release();
  void Render();
public:  
  /*Public Directives*/
private:
  /*Private Directives*/
  LPDIRECT3DDEVICE9 d_pDevice; //Pointer On Direct3D Device Interface
  ID3DXEffect* m_pEffect; //D3DX Effect Interface
  D3DXHANDLE m_pTechnique; //D3DX Technigue01 Handle
  CGRModels* m_pWeapon; //Weapon Model Class

  void CreateShader(TCHAR* EffectFile);
  void RefreshLights();
protected:
  /*Protected Directives*/
};
#endif //!defined(AFX_GRWeapon_H__)