/*******************************************************************************
// It's Unofficial Version Of The GreenRay Engine v.3.0.0.2
// GR_Particles.h: Interface For The CGRParticle Class.
// It's The GreenRay Physics Engine.
// Made Specially For Implementing Particle System.
// Date Creation: 29 August 2005
// Last Changing: 15 March 2009
// Licensed By: (C)DDD Digital Dreams Development And (C)3D Studios Inc., 2009
*******************************************************************************/
#include <GREngine.h>
#include <GRMethods.h>

////////////////////////////////////////////////////////////////////////////////
// Classes Used By This Header
////////////////////////////////////////////////////////////////////////////////
//class CGRParticle;

#if !defined(AFX_PARTICLE_H__)
#define AFX_PARTICLE_H__

////////////////////////////////////////////////////////////////////////////////
// Symbolic Constants
////////////////////////////////////////////////////////////////////////////////
//Classify Point
const int CP_FRONT   = 0;
const int CP_BACK    = 1;
const int CP_ONPLANE = 2;

//Collision Results
const int CR_BOUNCE  = 0;
const int CR_STICK   = 1;
const int CR_RECYCLE = 2;

////////////////////////////////////////////////////////////////////////////////
// Globals
////////////////////////////////////////////////////////////////////////////////
struct Plane
{
  D3DXVECTOR3 m_vNormal; //The Plane's Normal
  D3DXVECTOR3 m_vPoint; //A Coplanar Point Within The Plane
  float m_fBounceFactor; //Coefficient Of Restitution (Or How Bouncy The Plane Is)
  int m_nCollisionResult; //What Will Particles Do When They Strike The Plane
  Plane* m_pNext; //Next Plane In List
};

struct Particle
{
  D3DXVECTOR3 m_vCurPos; //Current Position Of Particle
  D3DXVECTOR3 m_vCurVel; //Current Velocity Of Particle
  float m_fInitTime; //Time Of Creation Of Particle
  Particle* m_pNext; //Next Particle In List
};

//Custom Vertex And FVF Declaration For Point Sprite Vertex Points
struct PointVertex
{
  D3DXVECTOR3 posit;
  D3DCOLOR color;

  enum FVF
  {
    FVF_Flags = D3DFVF_XYZ|D3DFVF_DIFFUSE
  };
};

//-----------------------------------------------------------------------------
// Name: Struct CGRParticle
// Desc: 
//-----------------------------------------------------------------------------
class CGRParticle
{	
public:
  /*Public Directives*/
  bool Is_Updated; //General Particle Update State Variable
  
  CGRParticle(LPDIRECT3DDEVICE9 pDevice);
  virtual ~CGRParticle();
  void Release();

  void SetTexture(char* chTexFile);
  LPDIRECT3DTEXTURE9 &GetTexture();
  void SetCollisionPlane(D3DXVECTOR3 vPlaneNormal,D3DXVECTOR3 vPoint, 
  float fBounceFactor = 1.0f,int nCollisionResult = CR_BOUNCE);
  HRESULT Init(); //Initialization Function
  HRESULT Update(); //Update Particle Function Every Frame
  void RestartParticleSystem(); //Restart Particle Animation
  HRESULT Render(); //Render Particles
  HRESULT RestoreDeviceObjects();
  HRESULT InvalidateDeviceObjects();

  void SetMaxParticles(DWORD dwMaxParticles) { m_dwMaxParticles = dwMaxParticles; }
  DWORD GetMaxParticles() { return m_dwMaxParticles; }

  void SetNumToRelease(DWORD dwNumToRelease) { m_dwNumToRelease = dwNumToRelease; }
  DWORD GetNumToRelease() { return m_dwNumToRelease; }

  void SetReleaseInterval(float fReleaseInterval) { m_fReleaseInterval = fReleaseInterval; }
  float GetReleaseInterval() { return m_fReleaseInterval; }

  void SetLifeCycle(float fLifeCycle) { m_fLifeCycle = fLifeCycle; }
  float GetLifeCycle() { return m_fLifeCycle; }

  void SetSize(float fSize) { m_fSize = fSize; }
  float GetSize() { return m_fSize; }
  float GetMaxPointSize() { return m_fMaxPointSize; }

  void SetColor(D3DXCOLOR clrColor) { m_clrColor = clrColor; }
  D3DXCOLOR GetColor() { return m_clrColor; }

  void SetPosition(D3DXVECTOR3 vPosition) { m_vPosition = vPosition; }
  D3DXVECTOR3 GetPosition() { return m_vPosition; }

  void SetVelocity(D3DXVECTOR3 vVelocity) { m_vVelocity = vVelocity; }
  D3DXVECTOR3 GetVelocity() { return m_vVelocity; }

  void SetGravity(D3DXVECTOR3 vGravity) { m_vGravity = vGravity; }
  D3DXVECTOR3 GetGravity() { return m_vGravity; }

  void SetWind(D3DXVECTOR3 vWind) { m_vWind = vWind; }
  D3DXVECTOR3 GetWind() { return m_vWind; }

  void SetAirResistence(bool bAirResistence) { m_bAirResistence = bAirResistence; }
  bool GetAirResistence() { return m_bAirResistence; }

  void SetVelocityVar(float fVelocityVar) { m_fVelocityVar = fVelocityVar; }
  float GetVelocityVar() { return m_fVelocityVar; }
public:
  /*Public Directives*/
private:
  /*Private Directives*/ 
  LPDIRECT3DDEVICE9 d_pDevice; //Pointer On Direct3D Device Interface
  LPDIRECT3DVERTEXBUFFER9 m_pVB; //Vertex Buffer For Point Sprites
  LPDIRECT3DTEXTURE9 m_pTexParticle; //Particles Texture

  DWORD       m_dwVBOffset;
  DWORD       m_dwFlush;
  DWORD       m_dwDiscard;
  Particle*   m_pActiveList;
  Particle*   m_pFreeList;
  Plane*      m_pPlanes;
  DWORD       m_dwActiveCount;
  float       m_fCurrentTime;
  float       m_fLastUpdate;
  float       m_fTimePassed;
  float       m_fMaxPointSize;
  bool        m_bDeviceSupportsPSIZE;
  
  //Particle Attributes
  DWORD       m_dwMaxParticles;
  DWORD       m_dwNumToRelease;
  float       m_fReleaseInterval;
  float       m_fLifeCycle;
  float       m_fSize;
  D3DXCOLOR   m_clrColor;
  D3DXVECTOR3 m_vPosition;
  D3DXVECTOR3 m_vVelocity;
  D3DXVECTOR3 m_vGravity;
  D3DXVECTOR3 m_vWind;
  bool        m_bAirResistence;
  float       m_fVelocityVar;
  char*       m_chTexFile;

  //Helper Functions
  int ClassifyPoint(D3DXVECTOR3* vPoint,Plane* pPlane);
  float GetRandomMinMax(float fMin,float fMax);
  D3DXVECTOR3 GetRandomVector();
};
#endif //!defined(AFX_PARTICLE_H__)