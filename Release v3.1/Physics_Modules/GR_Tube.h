/*******************************************************************************
// It's Unofficial Version Of The GreenRay Engine v.3.0.0.2
// GR_Tube.h: Interface For The TGRTube Class.
// It's The GreenRay Shotted Tube Engine.
// Made Specially For Implementing Shotted Tube Animation With Physics.
// Date Creation: 29 August 2005
// Last Changing: 07 April 2009
// Licensed By: (C)DDD Digital Dreams Development And (C)3D Studios Inc., 2009
*******************************************************************************/
#include <GREngine.h>
#include <GRMethods.h>
#include <GRSound.h>
#include <GR_Camera.h>

////////////////////////////////////////////////////////////////////////////////
// Classes Used By This Header
////////////////////////////////////////////////////////////////////////////////
//class CGTube;

//-----------------------------------------------------------------------------
// Name: Struct CGRTube
// Desc: 
//-----------------------------------------------------------------------------
#if !defined(AFX_TUBE_H__)
#define AFX_TUBE_H__
class CGRTube
{	
public:
  /*Public Directives*/
  CGRTube(LPDIRECT3DDEVICE9 pDevice,NxScene* pScene,LPCWSTR File_Name);
  virtual ~CGRTube();
  void Release();

  void Draw();
  NxVec3 ApplyForceToActor(NxActor* Actor,const NxVec3 &ForceDir,const NxReal ForceStrength);
  NxActor* GetActor();
  TCHAR* GetActorName();
  void ReleaseActor();
  void CreateShellSmoke();
public:
  /*Public Directives*/
  bool Dropped; //Drop Down State
  float Rotation; //Rotation Space
private:
  /*Private Directives*/  
  LPDIRECT3DDEVICE9 d_pDevice; //Pointer On Direct3D Device Interface
  NxScene* d_pScene; //Pointer On Scene PhysX Interface

  LPDIRECT3DVERTEXDECLARATION9 pVertDecl; // Vertex Declaration For Loadable Model
  LPCSTR m_pTexPath; //This Value Keep Texture Path
  CGRModel* m_pShell; //Shell Model Class
  CGRParticle* m_pShellSmoke; //Smoke Particle For Shell
  
  NxActor* m_pPhysXTube; //PhysX Actor Tube
  D3DXMATRIX mPos; //Global Object Position Matrix
  
  float Tube_Radius;
  float Tube_Height;
};
#endif //!defined(AFX_TUBE_H__)