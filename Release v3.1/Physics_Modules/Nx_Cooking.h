/*******************************************************************************
// It's Unofficial Version Of The GreenRay Engine v.3.0.0.2
// Nx_Cooking.h: Directives From The Novodex Class.
// It's The GreenRay Physics Engine.
// Made Specially For Implementing Physics And Collision Detection And Other.
// Date Creation: 29 August 2005
// Last Changing: 21 January 2007
// Licensed By: (C)DDD Digital Dreams Development And (C)3D Studios Inc., 2007
*******************************************************************************/
#pragma once
#include <GREngine.h>

//-----------------------------------------------------------------------------
class NxPMap;
class NxTriangleMesh;
class NxUserOutputStream;

bool hasCookingLibrary(); //Check To See If The Cooking Library Is Available Or Not!
bool InitCooking(NxUserAllocator* allocator = NULL,NxUserOutputStream* outputStream = NULL);
void CloseCooking();
bool CookConvexMesh(const NxConvexMeshDesc &desc,NxStream &stream);
bool CookClothMesh(const NxClothMeshDesc &desc,NxStream &stream);
bool CookTriangleMesh(const NxTriangleMeshDesc &desc,NxStream &stream);
bool CreatePMap(NxPMap &pmap,const NxTriangleMesh &mesh,NxU32 density,NxUserOutputStream* outputStream = NULL);
bool ReleasePMap(NxPMap &pmap);

//-----------------------------------------------------------------------------