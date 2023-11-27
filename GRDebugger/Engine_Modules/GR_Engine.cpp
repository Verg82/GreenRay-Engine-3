/****************************************************************************/ 
// It's Unofficial Version Of The GreenRay Engine v.3.0.0.2
// GR_Engine.cpp Visual Studio 8 Version.
// It's An GreenRay GREngine Source File.
// Made Specially For Implement 3D Engine Parameters.
// Date Creation: 29 August 2005
// Last Changing: 03 July 2009
// Licensed By: (C)DDD Digital Dreams Development And (C)3D Studios Inc., 2009
/*****************************************************************************/
//#pragma comment(lib,"d3d9.lib")
//#pragma comment(lib,"d3dx9.lib")
#include <GR_Engine.h>
#include <GR_Methods.h>
#include <GR_Camera.h>
#include <GR_SkyBox.h>

////////////////////////////////////////////////////////////////////////////////
// Global Variables 
////////////////////////////////////////////////////////////////////////////////
IDirect3DDevice9* m_pDevice = NULL; //The Direct3D Device Interface
HWND FormL_HWnd; //Form Layer Handle

CGRDeltaTimer* gr_Timer = NULL; //GreenRay Delta Timer Interface Class
CGRCamera* gr_Camera = NULL; //GreenRay Camera Global Interface Class
CGRSkyBox* gr_SkyBox = NULL; //GreenRay SkyBox Global Interface Class

//-----------------------------------------------------------------------------
// Name: CreateDirect3D()
// Desc: ��� ������� ������ �������� Direct3D ����������
//-----------------------------------------------------------------------------
HRESULT CreateDirect3D(HWND Hndl_Wnd)
{
  HRESULT m_pResult = S_OK; //Set Result By Default
  IDirect3D9* m_pDirect3D = NULL; //This General Direct3D Initialization Interface

  //������ ������ Direct3D
  m_pDirect3D = Direct3DCreate9(D3D_SDK_VERSION);

  //���������, ���� ��������� ������ �� ����������� D3D ���������
  if (!m_pDirect3D)
  {
    if (m_pDirect3D != NULL) m_pDirect3D->Release();
	MessageBoxA(NULL,"GR_Engine.cpp: Failed To Create Direct3D Interface..",
	"GreenRay Engine Error",MB_SYSTEMMODAL|MB_ICONERROR);
    m_pResult = E_FAIL;
  }

  //�������������� �������� ���������� �������������� ��������
  //����� ���������� ��������� ���������� � ��������� m_pDisplay
  D3DDISPLAYMODE m_pDisplay; 
  if (FAILED(m_pDirect3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT,&m_pDisplay)))
  {
    if (m_pDirect3D != NULL) m_pDirect3D->Release();
	MessageBoxA(NULL,"GR_Engine.cpp: Failed Get Adapter Display Mode..",
	"GreenRay Engine Error",MB_SYSTEMMODAL|MB_ICONERROR);
    m_pResult = E_FAIL;
  }

  //All System Checks Passed, Create The Direct3D Device
  D3DPRESENT_PARAMETERS m_pD3DParam;
  memset(&m_pD3DParam,0,sizeof(m_pD3DParam));

  //��������� ���������� � ������� ������ Direct3D ���������� 
  m_pD3DParam.SwapEffect                 = D3DSWAPEFFECT_DISCARD; //D3DSWAPEFFECT_COPY;
  
  //��������� ����������
  m_pD3DParam.BackBufferWidth            = 640; 
  m_pD3DParam.BackBufferHeight           = 480;
  //����� ����������� ����� ������ ���������� � ������ ����� (FALSE) ��� � ���� (TRUE)
  //�������� � ����� ������ ����� � ��� :-))
  m_pD3DParam.Windowed                   = TRUE;

  //������ ������� ������, ������ ������������ ������������� (m_pDisplay.Format), 
  //�� ����� ������� � ������� � ����������� �� ������������ �����
  m_pD3DParam.BackBufferFormat           = m_pDisplay.Format; //D3DFMT_A8R8G8B8 

  //������ �������� ���������� ����������������� ���� � ��� ������, ���� ���� 
  //���������� ����� �������� �� ������ ����� (����� ��� ������� ����������� 
  //������� �������� ��������) ������ �� ������� "��������" � ���� ����������, 
  //����� �� ������� ����������� ������������� (m_pDisplay.RefreshRate)
  //m_pD3DParam.FullScreen_RefreshRateInHz = m_pDisplay.RefreshRate; //D3DPRESENT_RATE_DEFAULT; 
  
  //��������� "������������" ������, � �������� ������������ � ���������� ��� ���������
  //�������� ����� � ������, ������ ��������� ��������� ��������� �������
  m_pD3DParam.EnableAutoDepthStencil     = TRUE;

  //����� ��� ����������� ������ "������������" ������
  m_pD3DParam.AutoDepthStencilFormat     = D3DFMT_D16; //D3DFMT_D16 D3DFMT_D15S1 D3DFMT_D24X8
                                                       //D3DFMT_D24S8 D3DFMT_D24X4S4 D3DFMT_D32
  
  //���������� ������ �������
  m_pD3DParam.BackBufferCount            = 2;

  //���-�� �������� ����������� ��� �������������
  m_pD3DParam.MultiSampleType            = D3DMULTISAMPLE_4_SAMPLES; //4_SAMPLES //For Antialiasing Mode On 4x
  
  //�������� �����������
  //m_pD3DParam.MultiSampleQuality       = 1;
  
  //����� ������� ������ ������������ ��� �������� ������� Shadow Volume "����" 
  //m_pD3DParam.Flags                    = D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL; //|D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
  
  //�������� ������������� ������������ �������� � ������� �����
  m_pD3DParam.PresentationInterval       = D3DPRESENT_INTERVAL_ONE;

  //���� ����������������� ���� ��������, �� �������� ����� ������� fps � ������
  //����� ����� ������������ ����������� �������, �.� ������ ���������� � �������� 
  //���������� ����� ����������� ��� ����������� ��������� � �������� ��� �� 100%
  //������ �������� ������������ �� �������������, ������� �������� �� �����..
  //�� � ������ ������������ ����� ����� �������� ��� ������ ����, 
  //����-�� �� ��������� ������ ����..
  //m_pD3DParam.PresentationInterval     = D3DPRESENT_INTERVAL_IMMEDIATE;

  FormL_HWnd = Hndl_Wnd; //Set Layer Window Handle

  //�������� Direct3D ����������, "�����������" ����� ������������� ��������� ������ ����������
  //����� ��������� Handle (Hndl_Wnd) ������ ����� ���������� ����, �� ���� ������� ���� ����� ���������� ������� 
  if (FAILED(m_pDirect3D->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,Hndl_Wnd,
  D3DCREATE_HARDWARE_VERTEXPROCESSING,&m_pD3DParam,&m_pDevice)))
  {
	//������ �������� Direct3D ���������� ����������� ���
    if (m_pDirect3D != NULL) m_pDirect3D->Release(); 	
	//������ ���� ��������� �������� ������, ������ �� "��������" ������ ���������� 
	//�������� ��� �� �������������� ����������� ���������, ���������� "����������" � ������� 
	//���������� ������������ � ����������, � ����� �� ������ ��������� "�����" �� ����������?? :-))))
	MessageBoxA(NULL,"GR_Engine.cpp: Error Creating Direct3D Device..",
	"GreenRay Engine Error",MB_SYSTEMMODAL|MB_ICONERROR);
    m_pResult = E_FAIL;
  }

  //Ending Direct3D Presets Parameters
  if (m_pDevice != NULL)
  {
	//��������� ������� �������:
    //������ ���������� � ������ ����� ��������� �������
    //��� ���� ������ ���������� "���������" ��������� ����� ������ (���������), �.�
    //������ ����������� � ������ ������� ������ ��� 3D �������, ��� �������� �� ���������
    //� �� �����������, ��� ��������� ����� ������ ���������� ������ ����� ���������� � ����
    //������� � �����.
    //����� D3DCULL_CW - ������������� ��������� ��������� ������
    //����� D3DCULL_C�W - ������������� ���� ��������� ������ ��� ������� ������ (��������)
	//����� D3DCULL_NONE - ��������� ��� ���� ������ ���������� ��� ������ ��� ���� ���� 
    //� ���������� �����, ��� ��� ������� ������ ��� ����������, ����� �� ��������� ������� fps
    //����� ��������� ����� ������.
	m_pDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_CW);
    
	//�������� ����� ������� ��� Z-Buffer, ��� ������ ��������� �������� ��� �������
	//"������" ������, �� �� ������ ������� ��������..
	m_pDevice->SetRenderState(D3DRS_ZENABLE,D3DZB_TRUE);

    //����� ���������, ��� �� ���� �� �����, ������� FALSE
    m_pDevice->SetRenderState(D3DRS_LIGHTING,FALSE); 
   
	//����� ������� ������������ �������� �� �������� SOLID, � � ������ ����� WIREFRAME
	//m_pDevice->SetRenderState(D3DRS_FILLMODE,D3DFILL_WIREFRAME);
    
	//��������� ����� ����������� (������) ���������
	//m_pDevice->SetRenderState(D3DRS_AMBIENT,0xFFFFFFFF);
    
	//������ ��������� �������� ����� ������ ������������� (��������� ������� ����������� 
	//���� ��� ���������� ���� �������� ���������� ��������������), ��� ���� �� ����� 
	//������� ���������������
	//m_pDevice->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS,TRUE);
	
	//��������� ������ �������� �� ����, ������ ��� ��������������� �� ��������� 
	//(�������� �� ���� ������ ����� ��� ����������� ������� ��� ������� ������������,
	//�������� ����� ��� ������� �� ���, � �� �� ��������� �����)
    //m_pDevice->SetRenderState(D3DRS_SHADEMODE,D3DSHADE_GOURAUD);



    //C����� ����� �������.
	gr_Timer = new CGRDeltaTimer(); //Construct A CGRDeltaTimer Class

	//C����� ����� ������
	gr_Camera = new CGRCamera(m_pDevice); //Construct A CGRCamera Class
	gr_Camera->SetCameraPos(D3DXVECTOR3(5.0f,50.78f,5.0f)); //Set Player Camera Position In Space
	gr_Camera->SetCameraSpeed(40.0f); //Set Player Camera Speed
    gr_Camera->SetMouseSpeed(0.3f); //Set Mouse Speed

    //Create SkyBox Class
	gr_SkyBox = new CGRSkyBox(m_pDevice); //Construct A CGRSkyBox Class  
    gr_SkyBox->CreateBox(100.0f,100.0f,100.0f); //Create Our SkyBox Model With Size WHD
    gr_SkyBox->SetTextures(TEXT("\Textures\\SkyBox\\Arch\\zneg.png"),
	                       TEXT("\Textures\\SkyBox\\Arch\\xneg.png"),
						   TEXT("\Textures\\SkyBox\\Arch\\zpos.png"),
						   TEXT("\Textures\\SkyBox\\Arch\\xpos.png"),
						   TEXT("\Textures\\SkyBox\\Arch\\yneg.png"),
						   TEXT("\Textures\\SkyBox\\Arch\\ypos.png"));
  } //End if (m_pDevice != NULL)

  if (m_pDirect3D != NULL) m_pDirect3D->Release(); //����������� Direct3D ��������� �� 
  //�������� ��� ��������������
  return m_pResult; //� ����� ���������� ��������� ������ �������
} //EndCreateDirect3DFunction

//-----------------------------------------------------------------------------
// Name: RenderingDirect3D()
// Desc: ����� � ���� ��������� ���������� ��������� ����� ������� �����
//-----------------------------------------------------------------------------
void RenderingDirect3D()
{   
  if (m_pDevice != NULL) 
  {
    if (gr_Timer != NULL) gr_Timer->UpdateTimer(); //Update GreenRay Timer Every Frame
	//���������� � ��������� ������ � ������, 
	//��� ��������� ��������� � ������ GR_Camera.cpp
	if (gr_Camera != NULL) gr_Camera->UpdateCamera(); //Update GreenRay Camera 

    //����� ��� ������� ������� � Z-������ ��������������..
    m_pDevice->Clear(0,NULL,D3DCLEAR_TARGET/*|D3DCLEAR_STENCIL*/,
	D3DCOLOR_XRGB(0,0,0),1.0f,0);
    
	//Begin Scene
    if (SUCCEEDED(m_pDevice->BeginScene()))
    {
      //�������� SkyBox
	  if (gr_SkyBox != NULL) gr_SkyBox->Render(); //Render The SkyBox ! 

	  m_pDevice->EndScene();
    }
    m_pDevice->Present(NULL,NULL,NULL,NULL);
  }
} //EndRenderingDirect3DProcedure

//-----------------------------------------------------------------------------
// Name: ReleaseDirect3D()
// Desc: ������������ �������� � ���������� Direct3D
//-----------------------------------------------------------------------------
void ReleaseDirect3D()
{
  if (gr_Camera != NULL) { /*gr_Camera->Release();*/ ReleaseClass(gr_Camera); DeleteClass(gr_Camera); } //Free Camera Class 
  if (gr_SkyBox != NULL) { gr_SkyBox->Release(); ReleaseClass(gr_SkyBox); DeleteClass(gr_SkyBox); } //Free SkyBox Class
  
  if (m_pDevice != NULL) m_pDevice->Release();
} //EndReleaseDirect3DProcedure

//------------------------------------------------------------------------------