/*******************************************************************************
// It's Unofficial Version Of The GreenRay Engine v.3.0.0.2
// GR_Decals.h:
// It's The GreenRay Physics Engine.
// Made Specially For Implementing Bullet Holes.
// Date Creation: 29 August 2005
// Last Changing: 09 March 2009
// Licensed By: (C)DDD Digital Dreams Development And (C)3D Studios Inc., 2009
*******************************************************************************/
#include <GREngine.h>
#include <GRMethods.h>
#include <GRSound.h>
#include <GR_Particles.h>

////////////////////////////////////////////////////////////////////////////////
// Classes Used By This Header
////////////////////////////////////////////////////////////////////////////////
//class CGRDecal;

//-----------------------------------------------------------------------------
// Name: Struct CGRDecal
// Desc: 
//-----------------------------------------------------------------------------
#if !defined(AFX_DECAL_H__)
#define AFX_DECAL_H__

#define D3DFVF_DECALVERTEX (D3DFVF_XYZ|D3DFVF_TEX1|D3DFVF_TEXCOORDSIZE3(0)) //Decal Vertex Format

class CGRDecal
{	
public:
  /*Public Directives*/
  //Decal Vertex, Normals, Texures Positions
  struct DECAL_VERTEX
  {
    FLOAT x,y,z;    //Coordinates
    FLOAT nx,ny,nz; //Normals
    DWORD _Color;   //Vertex Color
    FLOAT tu,tv;    //Textures
  }; //End Box Vertex Formats

  CGRDecal(LPDIRECT3DDEVICE9 pDevice,float Height,float Width,bool NoSmoke);
  virtual ~CGRDecal();
  void Release();

  HRESULT CreateDecalSurface(float fWidth,float fHeight);
  void SetCoord(D3DXVECTOR3 vPos,D3DXVECTOR3 vNormal,D3DXVECTOR3 vVec1,D3DXVECTOR3 vVec2);
  void Draw();
public:
  /*Public Directives*/
private:
  /*Private Directives*/ 
  LPDIRECT3DDEVICE9 d_pDevice; //Pointer On Direct3D Device Interface
  IDirect3DVertexBuffer9* m_pVertexBuffer; //The Vertex Buffer
  ID3DXMesh* m_pDecal; //Decal Interface
  CGRParticle* m_pParticle; //Particle System Interface
  CGRParticle* m_pSmoke; //Smoke Particle System Interface
  CGRParticle* m_pSpark; //Spark Particle System Interface

  D3DXVECTOR3 Pos,Normal,Vec_1,Vec_2; //Vectors And Normal For Orienting Decal
  D3DXVECTOR3 Par_Pos; //Particles Position
  D3DXMATRIX Mat_Trans; //Transform Matrices
  IDirect3DTexture9* m_pTexture; //Decal Texture
  DWORD dwColor; //Decal Texture Alpha Color
};
#endif //!defined(AFX_DECAL_H__)