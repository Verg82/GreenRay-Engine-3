/****************************************************************************/ 
// It's Unofficial Version Of The GreenRay Engine Debugger Module v.3.0.0.2
// Form_Layer.cpp Visual Studio 8 Version.
// It's An GreenRay Form_Layer Source File.
// Made Specially For Check And Debugging 3DEngine.
// Date Creation: 29 August 2005
// Last Changing: 08 February 2009
// Author Rights By: Verg Technology, Zombie Soft And Lucky's Flash Studio Inc.,
/*****************************************************************************/
#define STRICT //Обеспечивает переносимость кода, к примеру мы прогу пишем на другой
//винде 16 разрядной к примеру, а объявление этого параметра позволит избежать многих ошибок при 
//компиляции уже к примеру под 32 разрядной ОС.
//Использование STRICT позволяет: 
//1) выполнять строгую проверку типов; 
//2) корректировать и согласовывать описания типа параметра и возвращаемого значения; 
//3) создавать прототипы определений типов для функций обратного вызова (оконные, диалоговые и специальные процедуры); 
//4) согласовывать с ANSI описания структур COMM, DCB и COMSTAT. 
//ну и многое другое, не будем уже углубляться в эти программистские Debri.

#define WIN32_LEAN_AND_MEAN //Позволяет обоийти ошибки множественного объявления 
//стркутур при подключении заголовочных файлов уже имеющих эти структуры. 

#include "Form_Layer.h" //Объявляем здесь наш заголовочный Header-файл

////////////////////////////////////////////////////////////////////////////////
// External Definitions And Prototypes 
////////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK WindowProcessing(HWND Hndl_Wnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow);

//-----------------------------------------------------------------------------
// Name: WindowProcessing()
// Desc: Здесь обрабатываются оконные сообщения
//-----------------------------------------------------------------------------
LRESULT CALLBACK WindowProcessing(HWND Hndl_Wnd,UINT Msg,WPARAM wParam,LPARAM lParam)
{
  switch(Msg)
  {
    case WM_KEYDOWN:
	{
	  if (wParam == VK_ESCAPE) //Перехватили нажатие кнопки Esc на клавиатуре
	  {
	    PostQuitMessage(0); break; //Обрабатываем
	  }
	}
    break;

    case WM_CLOSE: //Перехватили момент закрытия окна при нажатии на крестике в углу экрана 
	{
	  PostQuitMessage(0); //Обрабатываем
	}
		
    case WM_DESTROY: //Перехватили момент закрытия окна через трей или диспетчер задач Windows
	{
      PostQuitMessage(0); //Обрабатываем
    }
    break;

	default:
	{
	  return DefWindowProc(Hndl_Wnd,Msg,wParam,lParam); //А иначе продолжаем обрабатывать сообщения
	}
	break;
  }
  return 0;

  //Вообще здесь можно обрабатывать любые оконные сообщения, по сути даже осуществить 
  //управление игровым персонажем, но мы будем использовать для этого DirectInput.
} //EndWindowProcessingFunction

//-----------------------------------------------------------------------------
// Name: WinMain()
// Desc: Эта функция предназначена для создания нашего окна
// Это является точкой входа для нашего приложения, иначе в процессе компиляции
// вернётся сообщение об ошибке: Entry point must be defined.
//-----------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,
LPSTR lpCmdLine,int nCmdShow)
{
  WNDCLASSEX WindowClass; //Структура необходимая для регистрации оконного класса 
  MSG uMsg; //Оконные сообщения
  HWND Hndl_Wnd = NULL; //Дескриптор оконного класса

  int FormWidth = 640; //Ширина нашего окна 
  int FormHeight = 480; //Высота нашего окна
  LPCSTR FormCaption = "GreenRay Engine v3.0.0.2"; //Название выводимое в заголовке окна

  memset(&uMsg,0,sizeof(uMsg)); //Передача в структуру флага для оконных сообщений

  //Установка параметров окна, таких как цвет, иконка, стиль окна, курсор
  //и передача их структуре WNDCLASSEX
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

  //Регистрация класса окна
  if (!RegisterClassEx(&WindowClass)) return E_FAIL;

  //Теперь когда класс зарегистрирован можно создавать само окно
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

  //Если создание окна прошло неудачно, то функция вернёт ошибку
  if (Hndl_Wnd == NULL) return E_FAIL;

  ShowWindow(Hndl_Wnd,nCmdShow); //Иначе покажем
  UpdateWindow(Hndl_Wnd); //и обновим наше окно

  //Создаём наше Direct3D устройство и укажем Handle нашего окна куда будет в 
  //дальнейшем выводиться графика
  CreateDirect3D(Hndl_Wnd);

  //Здесь наше окно начинает интенсивно обрабатывать сообщения от Windows
  //"крутим" этот цикл пока не поступит комманда закрытия нашего окна
  while (uMsg.message != WM_QUIT)
  {
    if (PeekMessage(&uMsg,NULL,0,0,PM_REMOVE)) //Нддаа если возникает это событие, то
		                                       //происходит уже явный перебор потока сообщений!
	{                                          //они начинают попросту удаляться из потока (данный манёвр 
		                                       //позволяет немного разгрузить процессор)
	  TranslateMessage(&uMsg); //Трансляция, перевод потока сообщений
	  DispatchMessage(&uMsg); //Обслуживание потока сообщений
	} 
	//Всё ОК рендерим нашу сцену
	else RenderingDirect3D();
  }

  ReleaseDirect3D(); //Освобождение ресурсов и выгрузка Direct3D
 
  //Выгрузка оконного класса он выполнил своё предназначение ;-)
  UnregisterClass("MY_WINDOWS_CLASS",WindowClass.hInstance);
  return uMsg.wParam;
} //EndWinMainFunction