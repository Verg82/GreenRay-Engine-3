/*******************************************************************************
// It's Unofficial Version Of The GreenRay Engine v.3.0.0.1
// GRCamera.cpp
// It's The GreenRay Direct3D Camera Control Interfaces Engine.
// Made Specially For Implementing Camera System Manipulations Parameters.
// Date Creation: 29 August 2005
// Last Changing: 23 March 2006
// Author Rights By: Zie Technology, Zombie Soft And Lucky's Flash Studio Inc.,
// Licensed By: (C)DDD Digital Dreams Development And (C)3D Studios Inc., 2006
*******************************************************************************/
#include <GRCamera.h>
#include <GRMethods.h>
//#include <GREffects.h>
//#include <GRInterface.h>

////////////////////////////////////////////////////////////////////////////////
// External Definitions And Prototypes 
////////////////////////////////////////////////////////////////////////////////
//DataFormat For DIMouseState
static DIOBJECTDATAFORMAT DIMouseFmt[] = {
	{ &GUID_XAxis, 0,  0x00FFFF03, 0L },
	{ &GUID_YAxis, 4,  0x00FFFF03, 0L },
	{ &GUID_ZAxis, 8,  0x80FFFF03, 0L },
	{ NULL,        12, 0x00FFFF0C, 0L },
	{ NULL,        13, 0x00FFFF0C, 0L },
	{ NULL,        14, 0x80FFFF0C, 0L },
	{ NULL,        15, 0x80FFFF0C, 0L }, 
};

const DIDATAFORMAT gr_DIMouseFromat = {24,16,2,16,7,DIMouseFmt};

//DataFormat For DIKeyboardState
#define DI_KEY_ID(Code)(0x80000000|DIDFT_BUTTON|DIDFT_MAKEINSTANCE(Code))
//#define DIPROP_BUFFERSIZE MAKEDIPROP(1)

static DIOBJECTDATAFORMAT DIKeyboardFmt[] = {
	{ &GUID_Key, 0,   DI_KEY_ID(0),   0L },	
	{ &GUID_Key, 1,   DI_KEY_ID(1),   0L },
	{ &GUID_Key, 2,   DI_KEY_ID(2),   0L },
	{ &GUID_Key, 3,   DI_KEY_ID(3),   0L },
	{ &GUID_Key, 4,   DI_KEY_ID(4),   0L },
	{ &GUID_Key, 5,   DI_KEY_ID(5),   0L },
	{ &GUID_Key, 6,   DI_KEY_ID(6),   0L },
	{ &GUID_Key, 7,   DI_KEY_ID(7),   0L },
	{ &GUID_Key, 8,   DI_KEY_ID(8),   0L },
	{ &GUID_Key, 9,   DI_KEY_ID(9),   0L },
	{ &GUID_Key, 10,  DI_KEY_ID(10),  0L },
	{ &GUID_Key, 11,  DI_KEY_ID(11),  0L },
	{ &GUID_Key, 12,  DI_KEY_ID(12),  0L },
	{ &GUID_Key, 13,  DI_KEY_ID(13),  0L },
	{ &GUID_Key, 14,  DI_KEY_ID(14),  0L },
	{ &GUID_Key, 15,  DI_KEY_ID(15),  0L },
	{ &GUID_Key, 16,  DI_KEY_ID(16),  0L },
	{ &GUID_Key, 17,  DI_KEY_ID(17),  0L },
	{ &GUID_Key, 18,  DI_KEY_ID(18),  0L },
	{ &GUID_Key, 19,  DI_KEY_ID(19),  0L },
	{ &GUID_Key, 20,  DI_KEY_ID(20),  0L },
	{ &GUID_Key, 21,  DI_KEY_ID(21),  0L },
	{ &GUID_Key, 22,  DI_KEY_ID(22),  0L },
	{ &GUID_Key, 23,  DI_KEY_ID(23),  0L },
	{ &GUID_Key, 24,  DI_KEY_ID(24),  0L },
	{ &GUID_Key, 25,  DI_KEY_ID(25),  0L },
	{ &GUID_Key, 26,  DI_KEY_ID(26),  0L },
	{ &GUID_Key, 27,  DI_KEY_ID(27),  0L },
	{ &GUID_Key, 28,  DI_KEY_ID(28),  0L },
	{ &GUID_Key, 29,  DI_KEY_ID(29),  0L },
	{ &GUID_Key, 30,  DI_KEY_ID(30),  0L },
	{ &GUID_Key, 31,  DI_KEY_ID(31),  0L },
	{ &GUID_Key, 32,  DI_KEY_ID(32),  0L },
	{ &GUID_Key, 33,  DI_KEY_ID(33),  0L },
	{ &GUID_Key, 34,  DI_KEY_ID(34),  0L },
	{ &GUID_Key, 35,  DI_KEY_ID(35),  0L },
	{ &GUID_Key, 36,  DI_KEY_ID(36),  0L },
	{ &GUID_Key, 37,  DI_KEY_ID(37),  0L },
	{ &GUID_Key, 38,  DI_KEY_ID(38),  0L },
	{ &GUID_Key, 39,  DI_KEY_ID(39),  0L },
	{ &GUID_Key, 40,  DI_KEY_ID(40),  0L },
	{ &GUID_Key, 41,  DI_KEY_ID(41),  0L },
	{ &GUID_Key, 42,  DI_KEY_ID(42),  0L },
	{ &GUID_Key, 43,  DI_KEY_ID(43),  0L },
	{ &GUID_Key, 44,  DI_KEY_ID(44),  0L },
	{ &GUID_Key, 45,  DI_KEY_ID(45),  0L },
	{ &GUID_Key, 46,  DI_KEY_ID(46),  0L },
	{ &GUID_Key, 47,  DI_KEY_ID(47),  0L },
	{ &GUID_Key, 48,  DI_KEY_ID(48),  0L },
	{ &GUID_Key, 49,  DI_KEY_ID(49),  0L },
	{ &GUID_Key, 50,  DI_KEY_ID(50),  0L },
	{ &GUID_Key, 51,  DI_KEY_ID(51),  0L },
	{ &GUID_Key, 52,  DI_KEY_ID(52),  0L },
	{ &GUID_Key, 53,  DI_KEY_ID(53),  0L },
	{ &GUID_Key, 54,  DI_KEY_ID(54),  0L },
	{ &GUID_Key, 55,  DI_KEY_ID(55),  0L },
	{ &GUID_Key, 56,  DI_KEY_ID(56),  0L },
	{ &GUID_Key, 57,  DI_KEY_ID(57),  0L },
	{ &GUID_Key, 58,  DI_KEY_ID(58),  0L },
	{ &GUID_Key, 59,  DI_KEY_ID(59),  0L },
	{ &GUID_Key, 60,  DI_KEY_ID(60),  0L },
	{ &GUID_Key, 61,  DI_KEY_ID(61),  0L },
	{ &GUID_Key, 62,  DI_KEY_ID(62),  0L },
	{ &GUID_Key, 63,  DI_KEY_ID(63),  0L },
	{ &GUID_Key, 64,  DI_KEY_ID(64),  0L },
	{ &GUID_Key, 65,  DI_KEY_ID(65),  0L },
	{ &GUID_Key, 66,  DI_KEY_ID(66),  0L },
	{ &GUID_Key, 67,  DI_KEY_ID(67),  0L },
	{ &GUID_Key, 68,  DI_KEY_ID(68),  0L },
	{ &GUID_Key, 69,  DI_KEY_ID(69),  0L },
	{ &GUID_Key, 70,  DI_KEY_ID(70),  0L },
	{ &GUID_Key, 71,  DI_KEY_ID(71),  0L },
	{ &GUID_Key, 72,  DI_KEY_ID(72),  0L },
	{ &GUID_Key, 73,  DI_KEY_ID(73),  0L },
	{ &GUID_Key, 74,  DI_KEY_ID(74),  0L },
	{ &GUID_Key, 75,  DI_KEY_ID(75),  0L },
	{ &GUID_Key, 76,  DI_KEY_ID(76),  0L },
	{ &GUID_Key, 77,  DI_KEY_ID(77),  0L },
	{ &GUID_Key, 78,  DI_KEY_ID(78),  0L },
	{ &GUID_Key, 79,  DI_KEY_ID(79),  0L },
	{ &GUID_Key, 80,  DI_KEY_ID(80),  0L },
	{ &GUID_Key, 81,  DI_KEY_ID(81),  0L },
	{ &GUID_Key, 82,  DI_KEY_ID(82),  0L },
	{ &GUID_Key, 83,  DI_KEY_ID(83),  0L },
	{ &GUID_Key, 84,  DI_KEY_ID(84),  0L },
	{ &GUID_Key, 85,  DI_KEY_ID(85),  0L },
	{ &GUID_Key, 86,  DI_KEY_ID(86),  0L },
	{ &GUID_Key, 87,  DI_KEY_ID(87),  0L },
	{ &GUID_Key, 88,  DI_KEY_ID(88),  0L },
	{ &GUID_Key, 89,  DI_KEY_ID(89),  0L },
	{ &GUID_Key, 90,  DI_KEY_ID(90),  0L },
	{ &GUID_Key, 91,  DI_KEY_ID(91),  0L },
	{ &GUID_Key, 92,  DI_KEY_ID(92),  0L },
	{ &GUID_Key, 93,  DI_KEY_ID(93),  0L },
	{ &GUID_Key, 94,  DI_KEY_ID(94),  0L },
	{ &GUID_Key, 95,  DI_KEY_ID(95),  0L },
	{ &GUID_Key, 96,  DI_KEY_ID(96),  0L },
	{ &GUID_Key, 97,  DI_KEY_ID(97),  0L },
	{ &GUID_Key, 98,  DI_KEY_ID(98),  0L },
	{ &GUID_Key, 99,  DI_KEY_ID(99),  0L },
	{ &GUID_Key, 100, DI_KEY_ID(100), 0L },
	{ &GUID_Key, 101, DI_KEY_ID(101), 0L },
	{ &GUID_Key, 102, DI_KEY_ID(102), 0L },
	{ &GUID_Key, 103, DI_KEY_ID(103), 0L },
	{ &GUID_Key, 104, DI_KEY_ID(104), 0L },
	{ &GUID_Key, 105, DI_KEY_ID(105), 0L },
	{ &GUID_Key, 106, DI_KEY_ID(106), 0L },
	{ &GUID_Key, 107, DI_KEY_ID(107), 0L },
	{ &GUID_Key, 108, DI_KEY_ID(108), 0L },
	{ &GUID_Key, 109, DI_KEY_ID(109), 0L },
	{ &GUID_Key, 110, DI_KEY_ID(110), 0L },
	{ &GUID_Key, 111, DI_KEY_ID(111), 0L },
	{ &GUID_Key, 112, DI_KEY_ID(112), 0L },
	{ &GUID_Key, 113, DI_KEY_ID(113), 0L },
	{ &GUID_Key, 114, DI_KEY_ID(114), 0L },
	{ &GUID_Key, 115, DI_KEY_ID(115), 0L },
	{ &GUID_Key, 116, DI_KEY_ID(116), 0L },
	{ &GUID_Key, 117, DI_KEY_ID(117), 0L },
	{ &GUID_Key, 118, DI_KEY_ID(118), 0L },
	{ &GUID_Key, 119, DI_KEY_ID(119), 0L },
	{ &GUID_Key, 120, DI_KEY_ID(120), 0L },
	{ &GUID_Key, 121, DI_KEY_ID(121), 0L },
	{ &GUID_Key, 122, DI_KEY_ID(122), 0L },
	{ &GUID_Key, 123, DI_KEY_ID(123), 0L },
	{ &GUID_Key, 124, DI_KEY_ID(124), 0L },
	{ &GUID_Key, 125, DI_KEY_ID(125), 0L },
	{ &GUID_Key, 126, DI_KEY_ID(126), 0L },
	{ &GUID_Key, 127, DI_KEY_ID(127), 0L },
	{ &GUID_Key, 128, DI_KEY_ID(128), 0L },
	{ &GUID_Key, 129, DI_KEY_ID(129), 0L },
	{ &GUID_Key, 130, DI_KEY_ID(130), 0L },
	{ &GUID_Key, 131, DI_KEY_ID(131), 0L },
	{ &GUID_Key, 132, DI_KEY_ID(132), 0L },
	{ &GUID_Key, 133, DI_KEY_ID(133), 0L },
	{ &GUID_Key, 134, DI_KEY_ID(134), 0L },
	{ &GUID_Key, 135, DI_KEY_ID(135), 0L },
	{ &GUID_Key, 136, DI_KEY_ID(136), 0L },
	{ &GUID_Key, 137, DI_KEY_ID(137), 0L },
	{ &GUID_Key, 138, DI_KEY_ID(138), 0L },
	{ &GUID_Key, 139, DI_KEY_ID(139), 0L },
	{ &GUID_Key, 140, DI_KEY_ID(140), 0L },
	{ &GUID_Key, 141, DI_KEY_ID(141), 0L },
	{ &GUID_Key, 142, DI_KEY_ID(142), 0L },
	{ &GUID_Key, 143, DI_KEY_ID(143), 0L },
	{ &GUID_Key, 144, DI_KEY_ID(144), 0L },
	{ &GUID_Key, 145, DI_KEY_ID(145), 0L },
	{ &GUID_Key, 146, DI_KEY_ID(146), 0L },
	{ &GUID_Key, 147, DI_KEY_ID(147), 0L },
	{ &GUID_Key, 148, DI_KEY_ID(148), 0L },
	{ &GUID_Key, 149, DI_KEY_ID(149), 0L },
	{ &GUID_Key, 150, DI_KEY_ID(150), 0L },
	{ &GUID_Key, 151, DI_KEY_ID(151), 0L },
	{ &GUID_Key, 152, DI_KEY_ID(152), 0L },
	{ &GUID_Key, 153, DI_KEY_ID(153), 0L },
	{ &GUID_Key, 154, DI_KEY_ID(154), 0L },
	{ &GUID_Key, 155, DI_KEY_ID(155), 0L },
	{ &GUID_Key, 156, DI_KEY_ID(156), 0L },
	{ &GUID_Key, 157, DI_KEY_ID(157), 0L },
	{ &GUID_Key, 158, DI_KEY_ID(158), 0L },
	{ &GUID_Key, 159, DI_KEY_ID(159), 0L },
	{ &GUID_Key, 160, DI_KEY_ID(160), 0L },
	{ &GUID_Key, 161, DI_KEY_ID(161), 0L },
	{ &GUID_Key, 162, DI_KEY_ID(162), 0L },
	{ &GUID_Key, 163, DI_KEY_ID(163), 0L },
	{ &GUID_Key, 164, DI_KEY_ID(164), 0L },
	{ &GUID_Key, 165, DI_KEY_ID(165), 0L },
	{ &GUID_Key, 166, DI_KEY_ID(166), 0L },
	{ &GUID_Key, 167, DI_KEY_ID(167), 0L },
	{ &GUID_Key, 168, DI_KEY_ID(168), 0L },
	{ &GUID_Key, 169, DI_KEY_ID(169), 0L },
	{ &GUID_Key, 170, DI_KEY_ID(170), 0L },
	{ &GUID_Key, 171, DI_KEY_ID(171), 0L },
	{ &GUID_Key, 172, DI_KEY_ID(172), 0L },
	{ &GUID_Key, 173, DI_KEY_ID(173), 0L },
	{ &GUID_Key, 174, DI_KEY_ID(174), 0L },
	{ &GUID_Key, 175, DI_KEY_ID(175), 0L },
	{ &GUID_Key, 176, DI_KEY_ID(176), 0L },
	{ &GUID_Key, 177, DI_KEY_ID(177), 0L },
	{ &GUID_Key, 178, DI_KEY_ID(178), 0L },
	{ &GUID_Key, 179, DI_KEY_ID(179), 0L },
	{ &GUID_Key, 180, DI_KEY_ID(180), 0L },
	{ &GUID_Key, 181, DI_KEY_ID(181), 0L },
	{ &GUID_Key, 182, DI_KEY_ID(182), 0L },
	{ &GUID_Key, 183, DI_KEY_ID(183), 0L },
	{ &GUID_Key, 184, DI_KEY_ID(184), 0L },
	{ &GUID_Key, 185, DI_KEY_ID(185), 0L },
	{ &GUID_Key, 186, DI_KEY_ID(186), 0L },
	{ &GUID_Key, 187, DI_KEY_ID(187), 0L },
	{ &GUID_Key, 188, DI_KEY_ID(188), 0L },
	{ &GUID_Key, 189, DI_KEY_ID(189), 0L },
	{ &GUID_Key, 190, DI_KEY_ID(190), 0L },
	{ &GUID_Key, 191, DI_KEY_ID(191), 0L },
	{ &GUID_Key, 192, DI_KEY_ID(192), 0L },
	{ &GUID_Key, 193, DI_KEY_ID(193), 0L },
	{ &GUID_Key, 194, DI_KEY_ID(194), 0L },
	{ &GUID_Key, 195, DI_KEY_ID(195), 0L },
	{ &GUID_Key, 196, DI_KEY_ID(196), 0L },
	{ &GUID_Key, 197, DI_KEY_ID(197), 0L },
	{ &GUID_Key, 198, DI_KEY_ID(198), 0L },
	{ &GUID_Key, 199, DI_KEY_ID(199), 0L },
	{ &GUID_Key, 200, DI_KEY_ID(200), 0L },
	{ &GUID_Key, 201, DI_KEY_ID(201), 0L },
	{ &GUID_Key, 202, DI_KEY_ID(202), 0L },
	{ &GUID_Key, 203, DI_KEY_ID(203), 0L },
	{ &GUID_Key, 204, DI_KEY_ID(204), 0L },
	{ &GUID_Key, 205, DI_KEY_ID(205), 0L },
	{ &GUID_Key, 206, DI_KEY_ID(206), 0L },
	{ &GUID_Key, 207, DI_KEY_ID(207), 0L },
	{ &GUID_Key, 208, DI_KEY_ID(208), 0L },
	{ &GUID_Key, 209, DI_KEY_ID(209), 0L },
	{ &GUID_Key, 210, DI_KEY_ID(210), 0L },
	{ &GUID_Key, 211, DI_KEY_ID(211), 0L },
	{ &GUID_Key, 212, DI_KEY_ID(212), 0L },
	{ &GUID_Key, 213, DI_KEY_ID(213), 0L },
	{ &GUID_Key, 214, DI_KEY_ID(214), 0L },
	{ &GUID_Key, 215, DI_KEY_ID(215), 0L },
	{ &GUID_Key, 216, DI_KEY_ID(216), 0L },
	{ &GUID_Key, 217, DI_KEY_ID(217), 0L },
	{ &GUID_Key, 218, DI_KEY_ID(218), 0L },
	{ &GUID_Key, 219, DI_KEY_ID(219), 0L },
	{ &GUID_Key, 220, DI_KEY_ID(220), 0L },
	{ &GUID_Key, 221, DI_KEY_ID(221), 0L },
	{ &GUID_Key, 222, DI_KEY_ID(222), 0L },
	{ &GUID_Key, 223, DI_KEY_ID(223), 0L },
	{ &GUID_Key, 224, DI_KEY_ID(224), 0L },
	{ &GUID_Key, 225, DI_KEY_ID(225), 0L },
	{ &GUID_Key, 226, DI_KEY_ID(226), 0L },
	{ &GUID_Key, 227, DI_KEY_ID(227), 0L },
	{ &GUID_Key, 228, DI_KEY_ID(228), 0L },
	{ &GUID_Key, 229, DI_KEY_ID(229), 0L },
	{ &GUID_Key, 230, DI_KEY_ID(230), 0L },
	{ &GUID_Key, 231, DI_KEY_ID(231), 0L },
	{ &GUID_Key, 232, DI_KEY_ID(232), 0L },
	{ &GUID_Key, 233, DI_KEY_ID(233), 0L },
	{ &GUID_Key, 234, DI_KEY_ID(234), 0L },
	{ &GUID_Key, 235, DI_KEY_ID(235), 0L },
	{ &GUID_Key, 236, DI_KEY_ID(236), 0L },
	{ &GUID_Key, 237, DI_KEY_ID(237), 0L },
	{ &GUID_Key, 238, DI_KEY_ID(238), 0L },
	{ &GUID_Key, 239, DI_KEY_ID(239), 0L },
	{ &GUID_Key, 240, DI_KEY_ID(240), 0L },
	{ &GUID_Key, 241, DI_KEY_ID(241), 0L },
	{ &GUID_Key, 242, DI_KEY_ID(242), 0L },
	{ &GUID_Key, 243, DI_KEY_ID(243), 0L },
	{ &GUID_Key, 244, DI_KEY_ID(244), 0L },
	{ &GUID_Key, 245, DI_KEY_ID(245), 0L },
	{ &GUID_Key, 246, DI_KEY_ID(246), 0L },
	{ &GUID_Key, 247, DI_KEY_ID(247), 0L },
	{ &GUID_Key, 248, DI_KEY_ID(248), 0L },
	{ &GUID_Key, 249, DI_KEY_ID(249), 0L },
	{ &GUID_Key, 250, DI_KEY_ID(250), 0L },
	{ &GUID_Key, 251, DI_KEY_ID(251), 0L },
	{ &GUID_Key, 252, DI_KEY_ID(252), 0L },
	{ &GUID_Key, 253, DI_KEY_ID(253), 0L },
	{ &GUID_Key, 254, DI_KEY_ID(254), 0L },
	{ &GUID_Key, 255, DI_KEY_ID(255), 0L },
};

const DIDATAFORMAT gr_DIKeyboardFromat = {24,16,2,256,256,DIKeyboardFmt};

//////////////////////////////////////////////////////////////////////////////
// Construction/Destruction For CGRCamera Class
//////////////////////////////////////////////////////////////////////////////
CGRCamera::CGRCamera(LPDIRECT3DDEVICE9 pDevice,HWND _HWnd,bool bConnect,bool bNoClip)
{
  d_pDevice = pDevice; //Apropritate Device Pointer To Class
  m_pDirectInput = NULL;
  m_pMouse = NULL;
  m_pKeyboard = NULL;
  m_fCameraSpeed = 0.01f;
  m_bConnected = false; //Connect State To Camera
  m_bNoClip = false; //NoClip Mode
  m_bVelocity = false; //Velocity State
  m_bUp = false; //Jump Up State
  
  CreateInputDevices(_HWnd); //Create Input Devices

  //Connecting States To Camera
  m_bConnected = bConnect; //Set Connect To Camera State
  m_bNoClip = bNoClip; //Set NoClip Mode

  if (m_bNoClip) _VecY = 0.1f;  
  else { _VecY = -0.018f; _YLocate = 1.78f; } //Reset _VecY Param
    
  if (m_bConnected)
  {
    m_pMouse->Acquire();
    m_pKeyboard->Acquire();	
  }
  else 
  {
    m_pMouse->Unacquire();
    m_pKeyboard->Unacquire();
  }
} //EndConstructionDirectives

CGRCamera::~CGRCamera()
{
} //EndDestructionDirectives

//-----------------------------------------------------------------------------
// Name: CGRCamera::Release()
// Desc: 
//-----------------------------------------------------------------------------
void CGRCamera::Release()
{
  m_pMouse->Unacquire(); //UnControl Mouse
  m_pKeyboard->Unacquire(); //UnControl Keyboard

  ReleaseClass(m_pMouse); //Release Mouse
  ReleaseClass(m_pKeyboard); //Release Keyboard

  ReleaseClass(m_pDirectInput); //Release DirectInput
} //EndReleaseCameraProcedure

//-----------------------------------------------------------------------------
// Name: CGRCamera::ResetCamera()
// Desc: 
//-----------------------------------------------------------------------------
void CGRCamera::ResetCamera()
{
  _VecX = 1.6f;
  _VecY = -0.018f;
  _XLocate = 5.0f;
  _YLocate = 1.78f;
  _ZLocate = 5.0f;
} //EndResetCameraProcedure

//-----------------------------------------------------------------------------
// Name: CGRCamera::CreateInputDevices()
// Desc: 
//-----------------------------------------------------------------------------
void CGRCamera::CreateInputDevices(HWND _HWnd)
{
  //Create InputDevice
  if (FAILED(DirectInput8Create(GetModuleHandle(NULL),DIRECTINPUT_VERSION,
  IID_IDirectInput8,(void**)&m_pDirectInput,NULL))) 
  {
    MessageBoxA(NULL,"Can't Create DirectInputContext","GreenRay Engine Error",
	MB_SYSTEMMODAL|MB_ICONEXCLAMATION); return;
  }

  //Create Mouse Input Device
  if (FAILED(m_pDirectInput->CreateDevice(GUID_SysMouse,&m_pMouse,NULL)))
  {
    MessageBoxA(NULL,"Can't Create MouseInputDevice","GreenRay Engine Error",
	MB_SYSTEMMODAL|MB_ICONEXCLAMATION); 
	m_pDirectInput->Release(); 
	return;
  }

  if (FAILED(m_pMouse->SetDataFormat(&gr_DIMouseFromat))) 
  {
    MessageBoxA(NULL,"Can't Set MouseDataFormat","GreenRay Engine Error",
	MB_SYSTEMMODAL|MB_ICONEXCLAMATION);
    m_pMouse->Release();
    m_pDirectInput->Release();
    m_pMouse = NULL;
    m_pDirectInput = NULL;
	return;
  }

  if (FAILED(m_pMouse->SetCooperativeLevel(_HWnd,DISCL_FOREGROUND|DISCL_EXCLUSIVE)))
  {
    MessageBoxA(NULL,"Can't Set Mouse CooperativeLevel","GreenRay Engine Error",
	MB_SYSTEMMODAL|MB_ICONEXCLAMATION);
    m_pMouse->Release();
	m_pDirectInput->Release();
	m_pMouse = NULL;
    m_pDirectInput = NULL;
	return;
  }

  //if (pMouse != NULL) pMouse->Acquire(); //Getting Mouse Control 

  //Create Keyboard Input Device
  if FAILED(m_pDirectInput->CreateDevice(GUID_SysKeyboard,&m_pKeyboard,NULL))
  {
    MessageBoxA(NULL,"Can't Create KeyboardInputDevice","GreenRay Engine Error",
	MB_SYSTEMMODAL|MB_ICONEXCLAMATION);
    m_pKeyboard->Release();
	m_pKeyboard = NULL;
	return;
  }
        
  if (FAILED(m_pKeyboard->SetDataFormat(&gr_DIKeyboardFromat))) 
  {
    MessageBoxA(NULL,"Can't Set KeyboardDataFormat","GreenRay Engine Error",
	MB_SYSTEMMODAL|MB_ICONEXCLAMATION);
    m_pMouse->Release();
	m_pKeyboard->Release();
	m_pDirectInput->Release();
    m_pDirectInput = NULL;
    return;
  }

  if (FAILED(m_pKeyboard->SetCooperativeLevel(_HWnd,DISCL_FOREGROUND|DISCL_NONEXCLUSIVE)))
  {
    MessageBoxA(NULL,"Can't Set Keyboard CooperativeLevel","GreenRay Engine Error",
	MB_SYSTEMMODAL|MB_ICONEXCLAMATION);
    m_pMouse->Release();
	m_pKeyboard->Release();
	m_pDirectInput->Release();
    m_pDirectInput = NULL;
	return;
  }

  /*DIPROPDWORD dipdw = {
	{
    sizeof(DIPROPDWORD),  //diph.dwSize
	sizeof(DIPROPHEADER), //diph.dwHeaderSize
	0,                    //diph.dwObj
	DIPH_DEVICE,},        //diph.dwHow
	DINPUT_BUFFERSIZE256, //dwData
  };
           
  if (FAILED(m_pKeyboard->SetProperty(DIPROP_BUFFERSIZE,&dipdw.diph)))
  {
    MessageBoxA(NULL,"Can't Set Property For KeyboardDevice","GreenRay Engine Error",
	MB_SYSTEMMODAL|MB_ICONEXCLAMATION);
    m_pMouse->Release();
	m_pKeyboard->Release();
	m_pDirectInput->Release();
    m_pDirectInput = NULL;
	return;
  } */

  //if (pKeyboard != NULL) pKeyboard->Acquire(); //Getting Keyboard Control 
} //EndCreateInputDevicesProcedure

//-----------------------------------------------------------------------------
// Name: CGRCamera::CameraMoveSpeed()
// Desc: 
//-----------------------------------------------------------------------------
void CGRCamera::CameraMoveSpeed(float MoveSpeed)
{
  //if (MoveSpeed <= 0.000f) m_fCameraSpeed = 0.001f;
//  /*else*/ if (MoveSpeed > 1.0f) m_fCameraSpeed = 0.99f;
  //else 
	  m_fCameraSpeed = MoveSpeed;
} //EndCameraMoveSpeedProcedure

//-----------------------------------------------------------------------------
// Name: CGRCamera::MoveCamera()
// Desc: 
//-----------------------------------------------------------------------------
void CGRCamera::MoveCamera(int _Direction)
{
  if (_Direction == gr_CameraUp) { //Move Forward
  _XLocate+= (float)cos(_VecX)*m_fCameraSpeed;
  if (m_bNoClip) { _YLocate+= _VecY*m_fCameraSpeed; } //For Fly Mode
  _ZLocate+= (float)sin(_VecX)*m_fCameraSpeed; } 

  if (_Direction == gr_CameraDown) { //Move Backward
  _XLocate-= (float)cos(_VecX)*m_fCameraSpeed;
  if (m_bNoClip) { _YLocate-= _VecY*m_fCameraSpeed; } //For Fly Mode
  _ZLocate-= (float)sin(_VecX)*m_fCameraSpeed; }

  if (_Direction == gr_CameraLeft) { //Strafe Left
  _XLocate-= (float)sin(_VecX)*m_fCameraSpeed;
  _ZLocate+= (float)cos(_VecX)*m_fCameraSpeed; }

  if (_Direction == gr_CameraRight) { //Strafe Right
  _XLocate+= (float)sin(_VecX)*m_fCameraSpeed;
  _ZLocate-= (float)cos(_VecX)*m_fCameraSpeed; }

  if (_Direction == gr_CameraJump) { //Camera Make Jump Up
  if (!m_bNoClip) m_bVelocity = true; }

  if (m_bNoClip) //For Fly Mode
  {
  }
  else	//For Walk Mode
  {  
//    if (fCameraSpeed >= 0.001f && !_bVelocity) PlaybackSound("Footsteps"); //Playing Player Footsteps
  }
} //EndMoveCameraProcedure

//-----------------------------------------------------------------------------
// Name: CGRCamera::GetMouseState()
// Desc: 
//-----------------------------------------------------------------------------
void CGRCamera::GetMouseState()
{ 
  //Donation: GetMouseState
  DIMOUSESTATE _MouseData; //Value Keep Mouse States
   
  if (m_pMouse != NULL && m_bConnected) //Check On Ready Mouse
  {         
    //Get Mouse Input Data
    ZeroMemory(&_MouseData,sizeof(_MouseData));      
    m_pMouse->GetDeviceState(sizeof(DIMOUSESTATE),&_MouseData);
    m_pMouse->Acquire();

    //Here Getting Mouse States:
    _VecX-= (float)_MouseData.lX*0.003f; //Get Mouse X-Axis   
    //_VecZ-= (float)_MouseData.lZ*0.01f; //Scroll Up-Down

    if (m_bNoClip) //For Fly Mode
    {
	  _VecY-= (float)_MouseData.lY*0.01f;  //Get Mouse Y-Axis
      if (_VecY >= D3DX_PI+2.0f) _VecY = D3DX_PI+2.0f; //Blocking YAxis Movement Through Up
      if (_VecY <= -D3DX_PI-2.0f) _VecY = -D3DX_PI-2.0f; //Blocking YAxis Movement Through Down
    }
	else //For Walk Mode
	{
	  _VecY-= (float)_MouseData.lY*0.0050f;  //Get Mouse Y-Axis	 
      if (_VecY >= D3DX_PI-0.5f) _VecY = D3DX_PI-0.5f; //Blocking YAxis Movement Through Up
      if (_VecY <= -D3DX_PI+0.5f) _VecY = -D3DX_PI+0.5f; //Blocking YAxis Movement Through Down
	}
   
	//Mouse Button States (0- It's Button Up)
    //if (_MouseData.rgbButtons[0x00] & 0x80) PlaybackSound("KogoTam"); 
    //if (_MouseData.rgbButtons[0x01] & 0x80) PlaybackSound("KudaTam"); 
    //if (_MouseData.rgbButtons[0x02] & 0x80) PlaybackSound("ZachemTam"); 
  }
} //EndGetMouseStateProcedure

//-----------------------------------------------------------------------------
// Name: CGRCamera::GetKeyboardState()
// Desc: 
//-----------------------------------------------------------------------------
void CGRCamera::GetKeyboardState()
{
  //Donation: GetKeyboardState
  static char _KeyBuffer[256]; //Buffer To Hold Keyboard States

  if (m_pKeyboard != NULL && m_bConnected) //Check On Ready Keyboard
  { 
    //Keyboard Input Data
    ZeroMemory(&_KeyBuffer,sizeof(_KeyBuffer));
    m_pKeyboard->GetDeviceState(sizeof(_KeyBuffer),_KeyBuffer);
    m_pKeyboard->Acquire();

    //Keyboard Contol Input Mode
    if (_KeyBuffer[DIK_W] & 0x80) MoveCamera(gr_CameraUp);
    if (_KeyBuffer[DIK_S] & 0x80) MoveCamera(gr_CameraDown);
    if (_KeyBuffer[DIK_A] & 0x80) MoveCamera(gr_CameraLeft);
    if (_KeyBuffer[DIK_D] & 0x80) MoveCamera(gr_CameraRight);
	if (_KeyBuffer[DIK_SPACE] & 0x80) MoveCamera(gr_CameraJump);
	
	//(~) Set Enable\Disable Console Key
    //if (_KeyBuffer[0x29] & 0x80) PlaybackSound("GdeKonsol");
  }
} //EndGetKeyboardStateProcedure

//-----------------------------------------------------------------------------
// Name: CGRCamera::SetCameraPerspective()
// Desc: 
//-----------------------------------------------------------------------------
void CGRCamera::SetCameraPerspective()
{
  //D3DXMATRIX _MatrixWorld,_MatrixView,_MatrixProjection;
  //D3DXVECTOR3 _EyePoint,_LookAtPoint,_UpVecPoint; 

  //World Matrix
  //D3DXMatrixIdentity(_MatrixWorld);
  //D3DXMatrixRotationX(_MatrixWorld,_AngleX);
  //D3DXMatrixRotationY(_MatrixWorld,_AngleY/100.0);
  //gr_Direct3DDevice.SetTransform(D3DTS_WORLD,_MatrixWorld);

  //Projection Matrix With FOV = 90'
  //int _BufferWidth = 800; //750
  //int _BufferHeight = 800; //670
  //int _AspectRatio = _BufferWidth/_BufferHeight;
  float Aspect_Ratio = 1.1f;

  //View Matrix
  m_fEyePoint =    D3DXVECTOR3(_XLocate,_YLocate,_ZLocate);

  //_YLocate+1.0f*(float)cos(_ZLocate) //Motaniye Bashkoi Pri Hodbe
 
  m_fLookAtPoint = D3DXVECTOR3(_XLocate+1.0f*(float)cos(_VecX),_YLocate+_VecY,_ZLocate+1.0f*(float)sin(_VecX));
  
  m_fUpVecPoint =  D3DXVECTOR3(0.0f,1.0f,0.0f);

  //Look Matrix On Left Handed System
  D3DXMatrixLookAtLH(&_MatrixView,&m_fEyePoint,&m_fLookAtPoint,&m_fUpVecPoint);
  d_pDevice->SetTransform(D3DTS_VIEW,&_MatrixView);

  //Projection Matrix
  D3DXMatrixPerspectiveFovLH(&_MatrixProjection,D3DX_PI/4,Aspect_Ratio,0.1f,10000.0f);
  d_pDevice->SetTransform(D3DTS_PROJECTION,&_MatrixProjection); 
} //EndSetCameraPerspectiveProcedure

//-----------------------------------------------------------------------------
// Name: CGRCamera::UpdateCamera()
// Desc: Poll The DirectInput Devices
//-----------------------------------------------------------------------------
void CGRCamera::UpdateCamera()
{
  d_pDevice->TestCooperativeLevel();
  GetMouseState();
  GetKeyboardState();
} //EndUpdateCameraProcedure

//-----------------------------------------------------------------------------
// Name: CGRCamera::UpdateDeltaTimer()
// Desc: Execute Each Frame To Establish Time Base
//-----------------------------------------------------------------------------
void CGRCamera::UpdateDeltaTimer()
{
  CurrentTime = timeGetTime();
  DeltaTime = ((float)CurrentTime-(float)LastTime)*0.001f;
  LastTime = timeGetTime();
} //EndUpdateDeltaTimerProcedure

//-----------------------------------------------------------------------------
// Name: CGRCamera::UpdateJumpVelocity()
// Desc: Execute Each Frame To Establish Time Base
//-----------------------------------------------------------------------------
void CGRCamera::UpdateVelocity()
{
  float _VelocityUp = 4.0f;
  float _VelocityDown = 4.0f;

  if (m_bVelocity && !m_bNoClip)
  {
    //Velocity Up
    if (!m_bUp) _YLocate+= ((float)sin(_YLocate))*_VelocityUp*DeltaTime; 

    //Velocity Down	
    if (m_bUp) _YLocate-= ((float)sin(_YLocate))*_VelocityDown*DeltaTime; 
  
    if (_YLocate >= 2.8f) m_bUp = true;
    if (_YLocate <= 1.78f) { m_bUp = false; m_bVelocity = false; }
  }
} //EndUpdateVelocityProcedure

//-----------------------------------------------------------------------------
// Name: OutputCameraPositionXY()
// Desc: 
//-----------------------------------------------------------------------------
void OutputCameraPositionXY()
{
  //OutputMouseXY(gr_Camera->_VecX,gr_Camera->_VecY); //Output Mouse Disposition
  //OutputDisposition(gr_Camera->_XLocate,gr_Camera->_YLocate,gr_Camera->_ZLocate); //Output Local Disposition
} //EndOutputCameraPositionXYProcedure

//--------------------------------------------------------------------------------
