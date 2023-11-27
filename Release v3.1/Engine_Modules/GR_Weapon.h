/*******************************************************************************
// It's Unofficial Version Of The GreenRay Engine v.3.0.0.3
// GR_Weapon.h
// It's The GreenRay Weapon Class Header File.
// Programming By: Verg
// Date Creation: 29 August 2005
// Last Changing: 26 August 2007
// Author Rights By: Zie Technology And Lucky's Flash Studio Inc.,
// Licensed By: (C)DDD Digital Dreams Development Inc., 2007
*******************************************************************************/
#include <GREngine.h>
#include <GRMethods.h>
#include <GR_Models.h>
#include <GRSound.h>
#include <GRLighting.h>
#include <GR_Camera.h>
#include <GR_Label.h>

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

//Define A FVF For Billboard
#define BILLBOARD_D3DFVF_VERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1)

class CGRWeapon
{	
public:
  /*Public Directives*/
  //Define A Custom Vertex For Our Billboard
  struct BILLBOARD_VERTEX
  {
    FLOAT x,y,z; //Position Of Vertex In Space
	DWORD Color; //Color Of Vertex
	FLOAT u,v;	 //Texture Coordinates
  };

  bool Be_Shoot; //Weapon Shoot State

  CGRWeapon(LPDIRECT3DDEVICE9 pDevice);
  virtual ~CGRWeapon();

  void Release();
  void SetTexturePath(LPCSTR Tex_Path);
  void Render();
  void WeaponShoot();
public:  
  /*Public Directives*/
private:
  /*Private Directives*/
  LPDIRECT3DDEVICE9 d_pDevice; //Pointer On Direct3D Device Interface
  LPDIRECT3DVERTEXDECLARATION9 pVertDecl; // Vertex Declaration For Loadable Model
  LPCSTR m_pTexPath; //This Value Keep Texture Path

  CGRModel* m_pWeapon; //Weapon Model Class
  TGRWave* m_pShoot; //Wave Directive For Playing Shoot Sound
  TGRWave* m_pEmpty; //Wave Directive For Gun Empty Sound
  TGRWave* m_pVitia; //Wave Directive For Vitia Sound
  CGRLabel* m_pHUDBullets; //Weapon Bullet Counter
  CGRLabel* m_pInfo; //Weapon Information Label
  TCHAR m_pBullStr[5]; //String For Keep Bullets Count

  int Num_Bullets; //Weapon Bullets Counter
  int Inc_Clicks;

  //Create An Fire Billboard
  IDirect3DVertexBuffer9* pVertexBuffer; //The Vertex Buffer
  IDirect3DTexture9* m_pTexture;
  DWORD dwColor;
  float m_nWidth;
  float m_nHeight;

  HRESULT CreateVertexBuffer();
  void RenderPrimitive();
protected:
  /*Protected Directives*/
};
#endif //!defined(AFX_GRWeapon_H__)

extern CGRWeapon* gr_Weapon; //Weapon Class