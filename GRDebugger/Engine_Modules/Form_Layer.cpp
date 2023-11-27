/****************************************************************************/ 
// It's Unofficial Version Of The GreenRay Engine Debugger Module v.3.0.0.2
// Form_Layer.cpp Visual Studio 8 Version.
// It's An GreenRay Form_Layer Source File.
// Made Specially For Check And Debugging 3DEngine.
// Date Creation: 29 August 2005
// Last Changing: 08 February 2009
// Author Rights By: Verg Technology, Zombie Soft And Lucky's Flash Studio Inc.,
/*****************************************************************************/
#define STRICT //������������ ������������� ����, � ������� �� ����� ����� �� ������
//����� 16 ��������� � �������, � ���������� ����� ��������� �������� �������� ������ ������ ��� 
//���������� ��� � ������� ��� 32 ��������� ��.
//������������� STRICT ���������: 
//1) ��������� ������� �������� �����; 
//2) �������������� � ������������� �������� ���� ��������� � ������������� ��������; 
//3) ��������� ��������� ����������� ����� ��� ������� ��������� ������ (�������, ���������� � ����������� ���������); 
//4) ������������� � ANSI �������� �������� COMM, DCB � COMSTAT. 
//�� � ������ ������, �� ����� ��� ����������� � ��� ��������������� Debri.

#define WIN32_LEAN_AND_MEAN //��������� ������� ������ �������������� ���������� 
//�������� ��� ����������� ������������ ������ ��� ������� ��� ���������. 

#include "Form_Layer.h" //��������� ����� ��� ������������ Header-����

////////////////////////////////////////////////////////////////////////////////
// External Definitions And Prototypes 
////////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK WindowProcessing(HWND Hndl_Wnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow);

//-----------------------------------------------------------------------------
// Name: WindowProcessing()
// Desc: ����� �������������� ������� ���������
//-----------------------------------------------------------------------------
LRESULT CALLBACK WindowProcessing(HWND Hndl_Wnd,UINT Msg,WPARAM wParam,LPARAM lParam)
{
  switch(Msg)
  {
    case WM_KEYDOWN:
	{
	  if (wParam == VK_ESCAPE) //����������� ������� ������ Esc �� ����������
	  {
	    PostQuitMessage(0); break; //������������
	  }
	}
    break;

    case WM_CLOSE: //����������� ������ �������� ���� ��� ������� �� �������� � ���� ������ 
	{
	  PostQuitMessage(0); //������������
	}
		
    case WM_DESTROY: //����������� ������ �������� ���� ����� ���� ��� ��������� ����� Windows
	{
      PostQuitMessage(0); //������������
    }
    break;

	default:
	{
	  return DefWindowProc(Hndl_Wnd,Msg,wParam,lParam); //� ����� ���������� ������������ ���������
	}
	break;
  }
  return 0;

  //������ ����� ����� ������������ ����� ������� ���������, �� ���� ���� ����������� 
  //���������� ������� ����������, �� �� ����� ������������ ��� ����� DirectInput.
} //EndWindowProcessingFunction

//-----------------------------------------------------------------------------
// Name: WinMain()
// Desc: ��� ������� ������������� ��� �������� ������ ����
// ��� �������� ������ ����� ��� ������ ����������, ����� � �������� ����������
// ������� ��������� �� ������: Entry point must be defined.
//-----------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,
LPSTR lpCmdLine,int nCmdShow)
{
  WNDCLASSEX WindowClass; //��������� ����������� ��� ����������� �������� ������ 
  MSG uMsg; //������� ���������
  HWND Hndl_Wnd = NULL; //���������� �������� ������

  int FormWidth = 640; //������ ������ ���� 
  int FormHeight = 480; //������ ������ ����
  LPCSTR FormCaption = "GreenRay Engine v3.0.0.2"; //�������� ��������� � ��������� ����

  memset(&uMsg,0,sizeof(uMsg)); //�������� � ��������� ����� ��� ������� ���������

  //��������� ���������� ����, ����� ��� ����, ������, ����� ����, ������
  //� �������� �� ��������� WNDCLASSEX
  WindowClass.lpszClassName = "MainWindow";
  WindowClass.cbSize        = sizeof(WNDCLASSEX);
  WindowClass.style         = CS_DBLCLKS|CS_OWNDC|CS_HREDRAW|CS_VREDRAW;
  WindowClass.lpfnWndProc   = WindowProcessing;
  WindowClass.hInstance     = hInstance;
  WindowClass.hIcon	        = ::LoadIcon(0,IDI_APPLICATION);
  WindowClass.hIconSm	    = ::LoadIcon(0,IDI_APPLICATION);
  WindowClass.hCursor       = LoadCursor(NULL,IDC_ARROW);
  WindowClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
  WindowClass.lpszMenuName  = NULL;
  WindowClass.cbClsExtra    = 0;
  WindowClass.cbWndExtra    = 0;

  //����������� ������ ����
  if (!RegisterClassEx(&WindowClass)) return E_FAIL;

  //������ ����� ����� ��������������� ����� ��������� ���� ����
  Hndl_Wnd = CreateWindowEx(NULL,              //Window Style
                         "MainWindow",         //Window Class Name 
                         FormCaption,          //Caption Of The Window
						 WS_OVERLAPPEDWINDOW,  //Window Style Criterium
						 100,100,              //Left And Top Corner On Desktop
                         FormWidth,FormHeight, //Width And Height Of The Window
						 NULL,                 //Descriptor Of Parent Window
						 NULL,                 //Descriptor Of The Window Menu
						 hInstance,            //Descriptor Of Example Application
						 NULL);                //Pointer On Data Created Window

  //���� �������� ���� ������ ��������, �� ������� ����� ������
  if (Hndl_Wnd == NULL) return E_FAIL;

  ShowWindow(Hndl_Wnd,nCmdShow); //����� �������
  UpdateWindow(Hndl_Wnd); //� ������� ���� ����

  //������ ���� Direct3D ���������� � ������ Handle ������ ���� ���� ����� � 
  //���������� ���������� �������
  CreateDirect3D(Hndl_Wnd);

  //����� ���� ���� �������� ���������� ������������ ��������� �� Windows
  //"������" ���� ���� ���� �� �������� �������� �������� ������ ����
  while (uMsg.message != WM_QUIT)
  {
    if (PeekMessage(&uMsg,NULL,0,0,PM_REMOVE)) //����� ���� ��������� ��� �������, ��
		                                       //���������� ��� ����� ������� ������ ���������!
	{                                          //��� �������� �������� ��������� �� ������ (������ ����� 
		                                       //��������� ������� ���������� ���������)
	  TranslateMessage(&uMsg); //����������, ������� ������ ���������
	  DispatchMessage(&uMsg); //������������ ������ ���������
	} 
	//�� �� �������� ���� �����
	else RenderingDirect3D();
  }

  ReleaseDirect3D(); //������������ �������� � �������� Direct3D
 
  //�������� �������� ������ �� �������� ��� �������������� ;-)
  UnregisterClass("MY_WINDOWS_CLASS",WindowClass.hInstance);
  return uMsg.wParam;
} //EndWinMainFunction