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
// Desc: Эта функция создаёт основное Direct3D устройство
//-----------------------------------------------------------------------------
HRESULT CreateDirect3D(HWND Hndl_Wnd)
{
  HRESULT m_pResult = S_OK; //Set Result By Default
  IDirect3D9* m_pDirect3D = NULL; //This General Direct3D Initialization Interface

  //Создаём объект Direct3D
  m_pDirect3D = Direct3DCreate9(D3D_SDK_VERSION);

  //Проверяем, если возникнет ошибка то освобождаем D3D интерфейс
  if (!m_pDirect3D)
  {
    if (m_pDirect3D != NULL) m_pDirect3D->Release();
	MessageBoxA(NULL,"GR_Engine.cpp: Failed To Create Direct3D Interface..",
	"GreenRay Engine Error",MB_SYSTEMMODAL|MB_ICONERROR);
    m_pResult = E_FAIL;
  }

  //Автоматическая проверка параметров поддерживаемых дисплеем
  //далее полученные параметры помещаются в стркутуру m_pDisplay
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

  //Установка параметров и режимов работы Direct3D устройства 
  m_pD3DParam.SwapEffect                 = D3DSWAPEFFECT_DISCARD; //D3DSWAPEFFECT_COPY;
  
  //Установка разрешения
  m_pD3DParam.BackBufferWidth            = 640; 
  m_pD3DParam.BackBufferHeight           = 480;
  //Здесь указывается режим работы устройства в полный экран (FALSE) или в окне (TRUE)
  //угадайте в каком режиме будет у нас :-))
  m_pD3DParam.Windowed                   = TRUE;

  //Формат заднего буфера, обычно определяется автоматически (m_pDisplay.Format), 
  //но можно указать и вручную в зависимости от поставленных целей
  m_pD3DParam.BackBufferFormat           = m_pDisplay.Format; //D3DFMT_A8R8G8B8 

  //Данный параметр необходимо раскомментировать лишь в том случае, если наше 
  //приложение будет работать на полный экран (здесь как правило указывается 
  //частота развёртки монитора) крайне не советую "играться" с этим параметром, 
  //пусть уж частота определится автоматически (m_pDisplay.RefreshRate)
  //m_pD3DParam.FullScreen_RefreshRateInHz = m_pDisplay.RefreshRate; //D3DPRESENT_RATE_DEFAULT; 
  
  //Включение "стенсельного" буфера, в основном предназначен в дальнейшем для отрисовки
  //эффектов теней и зеркал, короче повторяет очертания исходного объекта
  m_pD3DParam.EnableAutoDepthStencil     = TRUE;

  //Здесь уже указывается формат "стенсельного" буфера
  m_pD3DParam.AutoDepthStencilFormat     = D3DFMT_D16; //D3DFMT_D16 D3DFMT_D15S1 D3DFMT_D24X8
                                                       //D3DFMT_D24S8 D3DFMT_D24X4S4 D3DFMT_D32
  
  //Количество задних буферов
  m_pD3DParam.BackBufferCount            = 2;

  //Кол-во степеней сглаживания для антиалиасинга
  m_pD3DParam.MultiSampleType            = D3DMULTISAMPLE_4_SAMPLES; //4_SAMPLES //For Antialiasing Mode On 4x
  
  //Качество сглаживания
  //m_pD3DParam.MultiSampleQuality       = 1;
  
  //Буфер глубины обычно активируется для создания эффекта Shadow Volume "тень" 
  //m_pD3DParam.Flags                    = D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL; //|D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
  
  //Интервал представления отображаемых объектов и игровой сцены
  m_pD3DParam.PresentationInterval       = D3DPRESENT_INTERVAL_ONE;

  //Если раскомментировать этот параметр, то ощутится явный прирост fps и вообще
  //сцена начнёт отображаться существенно быстрее, т.е помимо видеокарты в процессе 
  //рендеринга начнёт участвовать ваш центральный процессор и загрузит его на 100%
  //Данный параметр использовать не рекомендуется, система работает на износ..
  //Да и вообще процессорное время лучше поберечь для других нужд, 
  //хотя-бы на обработку физики игры..
  //m_pD3DParam.PresentationInterval     = D3DPRESENT_INTERVAL_IMMEDIATE;

  FormL_HWnd = Hndl_Wnd; //Set Layer Window Handle

  //Создание Direct3D устройства, "скармливаем" ранее установленные параметры нашему устройству
  //Также указываем Handle (Hndl_Wnd) нашего ранее созданного окна, то бишь полотна куда будет выводиться графика 
  if (FAILED(m_pDirect3D->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,Hndl_Wnd,
  D3DCREATE_HARDWARE_VERTEXPROCESSING,&m_pD3DParam,&m_pDevice)))
  {
	//Ошибка создания Direct3D устройства освобождаем его
    if (m_pDirect3D != NULL) m_pDirect3D->Release(); 	
	//Вообще если возникает подобная ошибка, значит мы "скормили" нашему устройству 
	//неверные или не поддерживаемые видеокартой параметры, попробуйте "поиграться" с другими 
	//значениями выставляемые в параметрах, а может вы забыли поставить "дрова" на видеокарту?? :-))))
	MessageBoxA(NULL,"GR_Engine.cpp: Error Creating Direct3D Device..",
	"GreenRay Engine Error",MB_SYSTEMMODAL|MB_ICONERROR);
    m_pResult = E_FAIL;
  }

  //Ending Direct3D Presets Parameters
  if (m_pDevice != NULL)
  {
	//Установка режимов рендера:
    //Весьма интересный и нужный режим состояния рендера
    //при этом режиме происходит "отсечение" невидимых взору граней (полигонов), т.е
    //граней находящихся с задней стороны модели или 3D объекта, они попросту не ренерятся
    //и не отображаюся, это позволяет очень сильно разгрузить работу нашей видеокарты и всей
    //системы в целом.
    //Режим D3DCULL_CW - устанавливает отсечение невидимый граней
    //Режим D3DCULL_CСW - устанавливает тоже отсечение только уже видимых граней (инверсия)
	//Режим D3DCULL_NONE - отключает оба этих режима рендерится вся модель как есть даже 
    //её внутренние части, это уже слишком тяжело для видеокарты, сразу же ощущается падение fps
    //после установки этого режима.
	m_pDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_CW);
    
	//Включаем буфер глубины или Z-Buffer, или эффект отдаления объектов или моделей
	//"вглубь" экрана, да уж совсем образно объяснил..
	m_pDevice->SetRenderState(D3DRS_ZENABLE,D3DZB_TRUE);

    //Режим освещения, нам он пока не нужен, поэтому FALSE
    m_pDevice->SetRenderState(D3DRS_LIGHTING,FALSE); 
   
	//Режим рендера отображающий полигоны не цельными SOLID, а в режиме сетки WIREFRAME
	//m_pDevice->SetRenderState(D3DRS_FILLMODE,D3DFILL_WIREFRAME);
    
	//Установка цвета амбиентного (общего) освещения
	//m_pDevice->SetRenderState(D3DRS_AMBIENT,0xFFFFFFFF);
    
	//Данная установка включает режим работы антиалиасинга (прменения эффекта сглаживания 
	//рёбер или неровности краёв полигона называется антиалиасингом), нам пока не нужен 
	//поэтому закомментирован
	//m_pDevice->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS,TRUE);
	
	//Установка режима закраски по Гуро, обычно она устанавливается по умолчанию 
	//(закраска по Гуро обычно нужна для сглаживания одутлых или круглых поверхностей,
	//например чтобы шар походил на шар, а не на ребристый комок)
    //m_pDevice->SetRenderState(D3DRS_SHADEMODE,D3DSHADE_GOURAUD);



    //Cоздаём класс таймера.
	gr_Timer = new CGRDeltaTimer(); //Construct A CGRDeltaTimer Class

	//Cоздаём класс камеры
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

  if (m_pDirect3D != NULL) m_pDirect3D->Release(); //Освобождаем Direct3D интерфейс он 
  //выполнил своё предназначение
  return m_pResult; //В конце возвращаем результат работы функции
} //EndCreateDirect3DFunction

//-----------------------------------------------------------------------------
// Name: RenderingDirect3D()
// Desc: Здесь в этой процедуре происходит отрисовка нашей игровой сцены
//-----------------------------------------------------------------------------
void RenderingDirect3D()
{   
  if (m_pDevice != NULL) 
  {
    if (gr_Timer != NULL) gr_Timer->UpdateTimer(); //Update GreenRay Timer Every Frame
	//Обновление и получение данных с камеры, 
	//код процедуры находится в модуле GR_Camera.cpp
	if (gr_Camera != NULL) gr_Camera->UpdateCamera(); //Update GreenRay Camera 

    //Здесь идёт очистка заднего и Z-буфера соответственно..
    m_pDevice->Clear(0,NULL,D3DCLEAR_TARGET/*|D3DCLEAR_STENCIL*/,
	D3DCOLOR_XRGB(0,0,0),1.0f,0);
    
	//Begin Scene
    if (SUCCEEDED(m_pDevice->BeginScene()))
    {
      //Рендерим SkyBox
	  if (gr_SkyBox != NULL) gr_SkyBox->Render(); //Render The SkyBox ! 

	  m_pDevice->EndScene();
    }
    m_pDevice->Present(NULL,NULL,NULL,NULL);
  }
} //EndRenderingDirect3DProcedure

//-----------------------------------------------------------------------------
// Name: ReleaseDirect3D()
// Desc: Освобождение ресурсов и устройства Direct3D
//-----------------------------------------------------------------------------
void ReleaseDirect3D()
{
  if (gr_Camera != NULL) { /*gr_Camera->Release();*/ ReleaseClass(gr_Camera); DeleteClass(gr_Camera); } //Free Camera Class 
  if (gr_SkyBox != NULL) { gr_SkyBox->Release(); ReleaseClass(gr_SkyBox); DeleteClass(gr_SkyBox); } //Free SkyBox Class
  
  if (m_pDevice != NULL) m_pDevice->Release();
} //EndReleaseDirect3DProcedure

//------------------------------------------------------------------------------