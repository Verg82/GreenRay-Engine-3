/*******************************************************************************
// It's Unofficial Version Of The GreenRay Engine v.3.0.0.2
// Nx_Cooking.cpp
// It's The GreenRay Physics Engine.
// Made Specially For Implementing Physics And Collision Detection And Other.
// Date Creation: 29 August 2005
// Last Changing: 21 January 2007
// Licensed By: (C)DDD Digital Dreams Development And (C)3D Studios Inc., 2007
*******************************************************************************/
#include "Nx_Cooking.h"

//If On The Windows Or Linux Platform And 2.5.0 Or Higher, Use The Versioned Cooking
//Interface Via PhysXLoader
#if defined(WIN32)||defined(LINUX)
#if NX_SDK_VERSION_NUMBER >= 250
#define COOKING_INTERFACE 1
#endif
#endif

#ifdef COOKING_INTERFACE
NxCookingInterface* gCooking = 0;
#endif

//-----------------------------------------------------------------------------
// Name: hasCookingLibrary()
// Desc: Check To See If The Cooking Library Is Available Or Not!
//-----------------------------------------------------------------------------
bool hasCookingLibrary(void)
{
  bool ret = true;

  #ifdef COOKING_INTERFACE
  if (gCooking == 0)
  {
    gCooking = NxGetCookingLib(NX_PHYSICS_SDK_VERSION);
    if (gCooking == 0) ret = false;
  }
  #endif
  return ret;
} //EndhasCookingLibraryFunction

//-----------------------------------------------------------------------------
// Name: CookConvexMesh()
// Desc: 
//-----------------------------------------------------------------------------
bool CookConvexMesh(const NxConvexMeshDesc &desc,NxStream &stream)
{
  #ifdef COOKING_INTERFACE
  hasCookingLibrary();
  if (!gCooking) return false;
  return gCooking->NxCookConvexMesh(desc,stream);
  #else
  return NxCookConvexMesh(desc,stream);
  #endif
} //EndCookConvexMeshFunction

//-----------------------------------------------------------------------------
// Name: CookClothMesh()
// Desc: 
//-----------------------------------------------------------------------------
bool CookClothMesh(const NxClothMeshDesc &desc,NxStream &stream)
{
  #ifdef COOKING_INTERFACE
  hasCookingLibrary();
  if (!gCooking) return false;
  return gCooking->NxCookClothMesh(desc,stream);
  #else
  return NxCookClothMesh(desc,stream);
  #endif
} //EndCookClothMeshFunction

//-----------------------------------------------------------------------------
// Name: CookTriangleMesh()
// Desc: 
//-----------------------------------------------------------------------------
bool CookTriangleMesh(const NxTriangleMeshDesc &desc,NxStream &stream)
{
  #ifdef COOKING_INTERFACE
  hasCookingLibrary();
  if (!gCooking) return false;
  return gCooking->NxCookTriangleMesh(desc,stream);
  #else
  return NxCookTriangleMesh(desc,stream);
  #endif
} //EndCookTriangleMeshFunction

//-----------------------------------------------------------------------------
// Name: InitCooking()
// Desc: 
//-----------------------------------------------------------------------------
bool InitCooking(NxUserAllocator* allocator,NxUserOutputStream* outputStream)
{
  #ifdef COOKING_INTERFACE
  hasCookingLibrary();
  if (!gCooking) return false;
  return gCooking->NxInitCooking(allocator,outputStream);
  #else
  return NxInitCooking(allocator,outputStream);
  #endif
} //EndInitCookingFunction

//-----------------------------------------------------------------------------
// Name: CloseCooking()
// Desc: 
//-----------------------------------------------------------------------------
void CloseCooking()
{
  #ifdef COOKING_INTERFACE
  if (!gCooking) return;
  gCooking->NxCloseCooking();
  #else
  return NxCloseCooking();
  #endif
} //EndCloseCookingProcedure

//-----------------------------------------------------------------------------
// Name: CreatePMap()
// Desc: 
//-----------------------------------------------------------------------------
bool CreatePMap(NxPMap &pmap,const NxTriangleMesh &mesh,NxU32 density,NxUserOutputStream* outputStream)
{
  #ifdef COOKING_INTERFACE
  hasCookingLibrary();
  if (!gCooking) return false;
  return gCooking->NxCreatePMap(pmap,mesh,density,outputStream);
  #else
  return NxCreatePMap(pmap,mesh,density,outputStream);
  #endif
} //EndCreatePMapFunction

//-----------------------------------------------------------------------------
// Name: ReleasePMap()
// Desc: 
//-----------------------------------------------------------------------------
bool ReleasePMap(NxPMap &pmap)
{
  #ifdef COOKING_INTERFACE
  hasCookingLibrary();
  if (!gCooking) return false;
  return gCooking->NxReleasePMap(pmap);
  #else
  return NxReleasePMap(pmap);
  #endif
} //EndReleasePMapFunction

//-----------------------------------------------------------------------------