/****************************************************************************/ 
// It's Unofficial Version Of The GreenRay Engine v.3.0.0.2
// GREngine.h Visual Studio 8 Version.
// It's An GreenRay GREngine Header File.
// Made Specially For Implement 3D Engine Parameters.
// Date Creation: 29 August 2005
// Last Changing: 21 January 2007
// Licensed By: (C)DDD Digital Dreams Development And (C)3D Studios Inc., 2007
/*****************************************************************************/
#define STRICT
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

//Windows And System Directives
#include <windows.h>
#include <atlstr.h>   
#include <stdio.h>
#include <tchar.h>
#include <wchar.h>
#include <strsafe.h>

//Direct3D Directives
#include <d3d9.h>
#include <d3dx9.h>
#include <d3dx9core.h>
#include <d3dx9mesh.h>
#include <dinput.h>

//Sound System Directives
#include <mmsystem.h>
#include <dshow.h>
#include <dsound.h>
#include <dmusici.h>

//PhysX Directives
#include "NxPhysics.h"
#include "NxCooking.h"
#include "NxStream.h"
#include "NxPhysicsSDK.h"
#include "NxPMap.h"
#include "PhysXLoader.h"
#include <NxUserRaycastReport.h>

////////////////////////////////////////////////////////////////////////////////
// External Definitions And Prototypes 
////////////////////////////////////////////////////////////////////////////////
HRESULT CreateDirect3D(HWND _HWnd);
void RenderingDirect3D();
void ReleaseDirect3D();

////////////////////////////////////////////////////////////////////////////////
// Global Variables
////////////////////////////////////////////////////////////////////////////////
extern int ScreenWidth,ScreenHeight; //BackBuffer Screen Width And Height
extern HWND FormL_HWnd; //Global Form Layer HWnd Handle

