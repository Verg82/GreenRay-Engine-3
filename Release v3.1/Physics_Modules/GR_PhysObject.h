/*******************************************************************************
// It's Unofficial Version Of The GreenRay Engine v.3.0.0.2
// GR_PhysObject.h: Interface For Phys Object Loading Class.
// It's The GreenRay Physics Object Loading Engine.
// Date Creation: 29 August 2005
// Last Changing: 21 January 2008
// Licensed By: (C)DDD Digital Dreams Development And (C)3D Studios Inc., 2009
*******************************************************************************/
#include <GREngine.h>
#include <GR_Models.h>
#include <GRMethods.h>
#include <GR_Particles.h>

////////////////////////////////////////////////////////////////////////////////
// Classes Used By This Header
////////////////////////////////////////////////////////////////////////////////
//class CGRPhysObject;

//-----------------------------------------------------------------------------
// Name: Struct CGRPhysObject
// Desc: 
//-----------------------------------------------------------------------------
#if !defined(AFX_PHYSOBJ_H__)
#define AFX_PHYSOBJ_H__

class CGRPhysObject
{	
public:
  /*Public Directives*/
  CGRPhysObject(LPDIRECT3DDEVICE9 pDevice,NxScene* pScene);
  virtual ~CGRPhysObject();
  void Release();
  
  void Create(LPCWSTR File_Name);
  void SetTexturePath(LPCSTR Tex_Path);
  void Draw();
  void CreateSparks(D3DXVECTOR3 Pos);
  NxActor* GetActor();
  LPD3DXMESH GetMesh();
  void AddForce();
public:
  /*Public Directives*/
private:
  /*Private Directives*/ 
  LPDIRECT3DDEVICE9 d_pDevice; //Pointer On Direct3D Device Interface
  NxScene* d_pScene; //Pointer On Scene PhysX Interface
  
  LPCSTR m_pTexPath; //This Value Keep Texture Path

  LPDIRECT3DVERTEXDECLARATION9 m_pVertDecl; // Vertex Declaration For Loadable Model
  CGRModel* m_pModel; //Model Class
  NxActor* m_pActor;
  CGRParticle* m_pSpark; //Spark Particle System Interface
};
#endif //!defined(AFX_PHYSOBJ_H__)