/****************************************************************************/ 
// It's Unofficial Version Of The GreenRay Engine v.3.0.0.2
// GR_Engine.h Visual Studio 8 Version.
// It's An GreenRay GREngine Header File.
// Made Specially For Implement 3D Engine Parameters.
// Date Creation: 29 August 2005
// Last Changing: 03 July 2009
// Licensed By: (C)DDD Digital Dreams Development And (C)3D Studios Inc., 2009
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

////////////////////////////////////////////////////////////////////////////////
// External Definitions And Prototypes 
////////////////////////////////////////////////////////////////////////////////
HRESULT CreateDirect3D(HWND Hndl_Wnd);
void RenderingDirect3D();
void ReleaseDirect3D();

////////////////////////////////////////////////////////////////////////////////
// Global Variables
////////////////////////////////////////////////////////////////////////////////
extern HWND FormL_HWnd; //Global Form Layer HWnd Handle