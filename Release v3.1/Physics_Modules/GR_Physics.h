/*******************************************************************************
// It's Unofficial Version Of The GreenRay Engine v.3.0.0.2
// GR_Physics.h: Interface For The TGRPhysics Class.
// It's The GreenRay Physics Engine.
// Made Specially For Implementing Physics And Collision Detection.
// Date Creation: 29 August 2005
// Last Changing: 21 January 2007
// Licensed By: (C)DDD Digital Dreams Development And (C)3D Studios Inc., 2007
*******************************************************************************/
#pragma once
#include <GREngine.h>
#include <GRMethods.h>
#include <GRSound.h>
#include <GR_Camera.h>
#include <GR_Weapon.h>
#include <GR_Decals.h>
#include <GR_Tube.h>
#include <GR_PhysObject.h>
#include <Nx_Cooking.h>
#include <Nx_Stream.h>
#include <GR_SoundEffects.h>

////////////////////////////////////////////////////////////////////////////////
// GreenRay PhysX Constants
////////////////////////////////////////////////////////////////////////////////
#define GRPHYSX_ROOM 1
#define GRPHYSX_BARREL 2

////////////////////////////////////////////////////////////////////////////////
// Classes Used By This Header
////////////////////////////////////////////////////////////////////////////////
//class CGRPhysics;

//-----------------------------------------------------------------------------
// Name: Struct CGRPhysics
// Desc: 
//-----------------------------------------------------------------------------
#if !defined(AFX_PHYSICS_H__)
#define AFX_PHYSICS_H__
class CGRPhysics
{	
public:
  //Public Directives

  //ID3DXMesh* m_Mesh;

  CGRPhysics(LPDIRECT3DDEVICE9 pDevice);
  virtual ~CGRPhysics();
  void Release();
  
  D3DXVECTOR3 NxVec3_To_DXVec3(NxVec3 PhysXVec3);
  NxScene* GetPhysXScene() { return m_pScene; }
  NxActor* GetPhysXWorld() { return PhysXCube; }
  NxActor* GetPhysXPlayer() { return m_pPhysXPlayer; }

  void BeginUpdatePhysX();
  void EndUpdatePhysX();
  void UpdatePhysXPlayer();
  void JumpPhysXPlayer(float Jump_Speed);
  void TrianglesFromDXMesh(ID3DXMesh* m_pMesh);
  void TrianglesFromDXMeshVerb(ID3DXMesh* m_pMesh,NxActor* Actor,NxVec3 Pos);
  void GenerateTrianglesFromDXMesh(ID3DXMesh* &m_pMesh,NxActor* &PhysXActor,NxVec3 &Mesh_Pos);
  void SetActorPair(NxActor* &ActorA,NxActor* ActorB,DWORD Param);
  void SetRaycastGroup(NxActor* &Actor);
  void SetBodyFlag(NxActor* &Actor,NxBodyFlag Flag);
  
  bool LoadXFile(char* MeshFilename,ID3DXMesh* &Mesh);

  void SetActorGroup(NxActor* Actor,NxCollisionGroup Group);
  void Init(DWORD Param);
  void MakeRaycast(NxActor* Actor,NxU32 GroupFlag,bool RaycastAll);
  void RampBullet();
public:
  //Public Directives
private:
  //Private Directives 
  LPDIRECT3DDEVICE9 d_pDevice; //Pointer On Direct3D Device Interface
  NxPhysicsSDK* m_pPhysicsSDK; //General PhysX Interface
  NxScene* m_pScene; //General Scene PhysX Interface
  
  NxActor* m_pPhysXPlayer; //Actor Player PhysX Interface
  NxActor* PhysXCube; //Static Mesh Geometry Actor


  NxVec3* Vertices; //Keep Vertices Structure
  DWORD* Triangles; //Keep Indices Structure

  D3DXVECTOR3 Vec1,Vec2,Vec3; //Normalized Triangle Vectors !!!

  //Keep An Index For Each Material
  int DefaultMaterial;
  int FrictionlessMaterial; //Set Material For Player
};
#endif //!defined(AFX_PHYSICS_H__)

////////////////////////////////////////////////////////////////////////////////
// Global Variables Or Constants
////////////////////////////////////////////////////////////////////////////////
extern CGRPhysics* gr_PhysX; //GreenRay Physics Global Interface Class
extern CGRModel* m_pDecMesh;