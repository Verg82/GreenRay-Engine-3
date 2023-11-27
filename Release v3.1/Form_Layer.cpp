/****************************************************************************/ 
// It's Unofficial Version Of The GreenRay Engine Debugger Module v.3.0.0.1
// Form_Layer.cpp Visual Studio 8 Version.
// It's An GreenRay Form_Layer Source File.
// Made Specially For Check And Debugging 3DEngine.
// Date Creation: 29 August 2005
// Last Changing: 16 August 2006
// Author Rights By: Zie Technology, Zombie Soft And Lucky's Flash Studio Inc.,
// Licensed By: (C)DDD Digital Dreams Development And (C)3D Studios Inc., 2006
/*****************************************************************************/
#define STRICT
#define WIN32_LEAN_AND_MEAN

#include "Form_Layer.h"

////////////////////////////////////////////////////////////////////////////////
// External Definitions And Prototypes 
////////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK WindowProcessing(HWND _HWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow);

//-----------------------------------------------------------------------------
// Name: WindowProcessing()
// Desc: Here Processing The Window Messages
//-----------------------------------------------------------------------------
LRESULT CALLBACK WindowProcessing(HWND _HWnd,UINT Msg,WPARAM wParam,LPARAM lParam)
{
  switch(Msg)
  {
    case WM_KEYDOWN:
	{
	  if (wParam == VK_ESCAPE)
	  {
	    PostQuitMessage(0); break;
	  }
	}
    break;

    case WM_CLOSE:
	{
	  PostQuitMessage(0);
	}
		
    case WM_DESTROY:
	{
      PostQuitMessage(0);
    }
    break;

	default:
	{
	  return DefWindowProc(_HWnd,Msg,wParam,lParam );
	}
	break;
  }
  return 0;
} //EndWindowProcessingFunction

//-----------------------------------------------------------------------------
// Name: WinMain()
// Desc: This Function Need To Create Our Debugging Form Layer. 
// This Entry Point Of All Our Application
//-----------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
{
  WNDCLASSEX WindowClass; //Creation Window Class
  MSG uMsg; //Window Messages
  HWND _HWnd = NULL; //Create Form Layer Handle Descriptor

  int FormWidth = 1024; //Width Of Our Window 
  int FormHeight = 768; //Height Of Our Window
  LPCSTR FormCaption = "GreenRay Engine v3.0.0.1"; //Caption Of Our Window

  memset(&uMsg,0,sizeof(uMsg));

  //Setting Of Window Class Parameters WNDCLASSEX
  WindowClass.lpszClassName = "MainWindow";
  WindowClass.cbSize        = sizeof(WNDCLASSEX);
  WindowClass.style         = CS_DBLCLKS|CS_OWNDC|CS_HREDRAW|CS_VREDRAW;
  WindowClass.lpfnWndProc   = WindowProcessing;
  WindowClass.hInstance     = hInstance;
  WindowClass.hIcon	        = ::LoadIcon(0,IDI_APPLICATION);
  WindowClass.hIconSm	    = ::LoadIcon(0,IDI_APPLICATION);
  WindowClass.hCursor       = LoadCursor(NULL,IDC_ARROW);
  WindowClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
  WindowClass.lpszMenuName  = NULL;
  WindowClass.cbClsExtra    = 0;
  WindowClass.cbWndExtra    = 0;

  //Registering Of Our Window Class
  if (!RegisterClassEx(&WindowClass)) return E_FAIL;

  //Then When Class Registered We Are Can Create The Same Window
  _HWnd = CreateWindowEx(NULL,                 //Window Style
                         "MainWindow",         //Window Class Name
                         FormCaption,          //Caption Of The Window
						 WS_OVERLAPPEDWINDOW,  //Window Style Criterium
						 100,100,              //Left And Top Corner On Desktop
                         FormWidth,FormHeight, //Width And Height Of The Window
						 NULL,                 //Descriptor Of Parent Window
						 NULL,                 //Descriptor Of The Window Menu
						 hInstance,            //Descriptor Of Example Application
						 NULL);                //Pointer On Data Created Window

  //If The Window Is Not Created Then Return NULL
  if (_HWnd == NULL) return E_FAIL;

  ShowWindow(_HWnd,nCmdShow); //Else Show Our Created Window
  UpdateWindow(_HWnd); //Update Our Window For Prepare Them For Draw

  CreateDirect3D(_HWnd); //Create Our Direct3D Interface 

  //Out Window Here Startup Intensively Getting Messages From Windows
  while (uMsg.message != WM_QUIT)
  {
    if (PeekMessage(&uMsg,NULL,0,0,PM_REMOVE))
	{ 
	  TranslateMessage(&uMsg); DispatchMessage(&uMsg);
	} 
	else RenderingDirect3D(); //Rendering Our Direct3D Scene
  }
 
  ReleaseDirect3D(); //Free Resources And Release Our Direct3D Interface

  //Unregistering Window Class If This Don't Needed
  UnregisterClass("MY_WINDOWS_CLASS",WindowClass.hInstance);
  return uMsg.wParam;
} //EndWinMainFunction