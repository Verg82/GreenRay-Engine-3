/*******************************************************************************
// It's Unofficial Version Of The GreenRay Engine v.3.0.0.2
// GR_SkyBox.cpp
// It's The GreenRay SkyBox Geometry Transform Engine Source File.
// Made Specially For Implementing Only SkyBox Geometry And Effects.
// Programming By: Verg
// Date Creation: 29 August 2005
// Last Changing: 03 July 2009
// Author Rights By: Zie Technology And Lucky's Flash Studio Inc.,
// Licensed By: (C)DDD Digital Dreams Development And (C)3D Studios Inc., 2009
*******************************************************************************/
#include <GR_SkyBox.h>

//////////////////////////////////////////////////////////////////////////////
// Construction/Destruction For CGRSkyBox Class
//////////////////////////////////////////////////////////////////////////////
CGRSkyBox::CGRSkyBox(LPDIRECT3DDEVICE9 pDevice)
{
  //Присваивание и "передача" указателя Direct3D Device в наш класс
  d_pDevice = pDevice; //To Appropriate Device Pointer To Class
  
  //Подготовка вершинного буфера
  m_pVertexBuffer = NULL; //Prepare Vertex Buffer

  //Подготовка индексного буфера
  m_pIndexBuffer = NULL; //Prepare Index Buffer

  //Подготовка текстур, присваивание массиву текстур значения
  //NULL как видите идёт одной строкой:
  memset(m_pTex,0x00,sizeof(m_pTex)); 
} //EndConstructionDirectives

CGRSkyBox::~CGRSkyBox()
{  
} //EndDestructionDirectives

//------------------------------------------------------------------------------
// Name: CGRSkyBox::Release()
// Desc: 
//------------------------------------------------------------------------------
void CGRSkyBox::Release()
{
  for (UINT I = 0; I < 6; I++) { ReleaseClass(m_pTex[I]); m_TexPath[I] = NULL; } //Free Textures
  ReleaseClass(m_pVertexBuffer); //Free Vertex Buffer
  ReleaseClass(m_pIndexBuffer); //Free Index Buffer
  d_pDevice = NULL; //NULL Device Pointer
} //EndReleaseProcedure

//-----------------------------------------------------------------------------
// Name: CGRSkyBox::SetTextures()
// Desc: Loading And Init Textures
//-----------------------------------------------------------------------------
void CGRSkyBox::SetTextures(TCHAR* _TexBack,TCHAR* _TexLeft,
TCHAR* _TexFront,TCHAR* _TexRight,TCHAR* _TexFloor,TCHAR* _TexCeiling)
{
  m_TexPath[0] = _TexBack;
  m_TexPath[1] = _TexLeft;
  m_TexPath[2] = _TexFront;
  m_TexPath[3] = _TexRight;
  m_TexPath[4] = _TexFloor;
  m_TexPath[5] = _TexCeiling;

  //Create Some Textures
  for (UINT I = 0; I < 6; I++)
  {
    if (FAILED(D3DXCreateTextureFromFileEx(d_pDevice,m_TexPath[I],D3DX_DEFAULT,D3DX_DEFAULT,
    1,0,D3DFMT_UNKNOWN,D3DPOOL_MANAGED,D3DX_DEFAULT,D3DX_DEFAULT,0,NULL,NULL,&m_pTex[I])))
	{
	  ShowMessage("GR_SkyBox.cpp: Can't Load SkyBox Texture");
	}
  }
} //EndSetTexturesProcedure

//-----------------------------------------------------------------------------
// Name: CGRSkyBox::CreateBox()
// Desc: Создание "коробки" SkyBox-а с инициализацией вершинного и индексного
// буфера.
//-----------------------------------------------------------------------------
HRESULT CGRSkyBox::CreateBox(float fWidth,float fHeight,float fDepth)
{
  m_pWidth = fWidth;
  m_pHeight = fHeight;
  m_pDepth = fDepth;

  //Массив с описанием 6 сторон куба в данной структуре вершин 
  //описываются координаты вершин в пространстве и текстурные
  //"барицентрические" (Barycentric) TU - TV координаты.
  SKYBOX_VERTEX Vertexes[] =
  {
    //Width   Height   Depth  tu(W) tv(H)
    //Back Wall
	{ fWidth, 0.0f,    0.0f, 0.0f, 1.0f, }, //A
    { fWidth, fHeight, 0.0f, 0.0f, 0.0f, }, //B
    { 0.0f,   fHeight, 0.0f, 1.0f, 0.0f, }, //C
	{ 0.0f,   0.0f,    0.0f, 1.0f, 1.0f, }, //D
   
	//Left Wall
    { 0.0f, 0.0f,    0.0f,   0.0f, 1.0f, }, //A2
    { 0.0f, fHeight, 0.0f,   0.0f, 0.0f, }, //B2
    { 0.0f, fHeight, fDepth, 1.0f, 0.0f, }, //C2
	{ 0.0f, 0.0f,    fDepth, 1.0f, 1.0f, }, //D2
    
	//Front Wall
	{ 0.0f,   0.0f,    fDepth, 0.0f, 1.0f, }, //A3
    { 0.0f,   fHeight, fDepth, 0.0f, 0.0f, }, //B3
    { fWidth, fHeight, fDepth, 1.0f, 0.0f, }, //C3
	{ fWidth, 0.0f,    fDepth, 1.0f, 1.0f, }, //D3
    
	//Right Wall
    { fWidth, 0.0f,    fDepth, 0.0f, 1.0f, }, //A4  
    { fWidth, fHeight, fDepth, 0.0f, 0.0f, }, //B4
    { fWidth, fHeight, 0.0f,   1.0f, 0.0f, }, //C4
	{ fWidth, 0.0f,    0.0f,   1.0f, 1.0f, }, //D4
    
	//Floor    
    { fWidth, 0.0f, 0.0f,   1.0f, 1.0f, }, //A5
    { 0.0f,   0.0f, 0.0f,   0.0f, 1.0f, }, //B5
    { 0.0f,   0.0f, fDepth, 0.0f, 0.0f, }, //C5
	{ fWidth, 0.0f, fDepth, 1.0f, 0.0f, }, //D5
     
	//Ceiling
    { fWidth, fHeight, fDepth, 1.0f, 1.0f, }, //A6 
    { 0.0f,   fHeight, fDepth, 0.0f, 1.0f, }, //B6
    { 0.0f,   fHeight, 0.0f,   0.0f, 0.0f, }, //C6
	{ fWidth, fHeight, 0.0f,   1.0f, 0.0f, }, //D6
    //  x        y       z     tu(W) tv(H)
  }; //End Vertexes Description

  //Массив индексов содержащий и описывающий 6 сторон куба (12 треугольников)
  const unsigned short Index[] = 
  {
  0,1,2,    2,3,0,
  4,5,6,    6,7,4,
  8,9,10,   10,11,8,
  12,13,14, 14,15,12,
  16,17,18, 18,19,16,
  20,21,22, 22,23,20, 
  }; //End Indexes Description
	
  //Create Vertex Buffer
  //Создание буфера вершин
  if (FAILED(d_pDevice->CreateVertexBuffer(36*sizeof(SKYBOX_VERTEX),0, 
  D3DFVF_SKYBOXVERTEX,D3DPOOL_DEFAULT,&m_pVertexBuffer,NULL))) return E_FAIL;

  //Lock The Vertex Buffer
  //Блокируем вершинный буфер
  VOID* pBV;
  if (FAILED(m_pVertexBuffer->Lock(0,sizeof(Vertexes),(void**)&pBV,0))) return E_FAIL;
  
  //Заполнение вершинного буфера данными
  memcpy(pBV,Vertexes,sizeof(Vertexes)); //Copy Vertex Data To Memory

  //Разблокируем вершинный буфер
  m_pVertexBuffer->Unlock(); //Unlock The Vertex Buffer
    
  //Create Index Buffer
  //Создание индексного буфера, здесь всё происходит 
  //примерно тоже самое, что и при создании вершинного буфера.
  d_pDevice->CreateIndexBuffer(36*sizeof(Index),0,D3DFMT_INDEX16,D3DPOOL_DEFAULT,&m_pIndexBuffer,NULL);
    
  //Lock The Index Buffer
  VOID* pBI;
  m_pIndexBuffer->Lock(0,sizeof(Index),(void**)&pBI,0); 
    
  memcpy(pBI,Index,sizeof(Index)); //Copy Index Data To Memory
      
  m_pIndexBuffer->Unlock(); //Unlock The Index Buffer
 
  return S_OK;
} //EndCreateSkyBoxFunction

//-----------------------------------------------------------------------------
// Name: CGRSkyBox::Render()
// Desc: Rendering The SkyBox Geometry With Textures
//-----------------------------------------------------------------------------
void CGRSkyBox::Render()
{  
  //Устанавливаем параметры рендера и отключаем Z-Буфер
  d_pDevice->SetRenderState(D3DRS_ZENABLE,FALSE);
  d_pDevice->SetRenderState(D3DRS_ZWRITEENABLE,FALSE);
  d_pDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_CCW);
  d_pDevice->SetRenderState(D3DRS_LIGHTING,FALSE);

  //VU Addr Задаём параметры текстурного сэмплера.
  d_pDevice->SetSamplerState(0,D3DSAMP_ADDRESSU,D3DTADDRESS_CLAMP);
  d_pDevice->SetSamplerState(0,D3DSAMP_ADDRESSV,D3DTADDRESS_CLAMP);

  //Set Texture Mode
  //d_pDevice->SetTextureStageState(0,D3DTSS_COLORARG1,D3DTA_TEXTURE);
  //d_pDevice->SetTextureStageState(0,D3DTSS_COLOROP,D3DTOP_MODULATE);	
  //d_pDevice->SetTextureStageState(0,D3DTSS_COLOROP,D3DTOP_SELECTARG1);
  d_pDevice->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR);
  d_pDevice->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_LINEAR);
  //d_pDevice->SetSamplerState(0,D3DSAMP_MIPFILTER,D3DTEXF_POINT);

  //Set Perspective View For Spherical Show SkyBox In Space (60 Degree) 
  //Установка камеры и "перспективы" FOV - Field Of View (область обзора)
  //в сферическое представление с углом в 60 градусов.
  float Aspect_Ratio = 1.1f;
  D3DXMATRIXA16 Mat_Proj;
  //FLOAT fAspectRatio = (float)m_d3dsdBackBuffer.Width/(float)m_d3dsdBackBuffer.Height;
  D3DXMatrixPerspectiveFovLH(&Mat_Proj,/*D3DX_PI/4*/D3DXToRadian(60.0f),Aspect_Ratio,1.0f,1000.0f);
  d_pDevice->SetTransform(D3DTS_PROJECTION,&Mat_Proj); 

  //Translation Matrices For The SkyBox Object
  //Устанавливаем размеры, позицию в центр, и привязываем SkyBox к камере,
  //чтобы SkyBox как бы двигался вместе с игроком и сам игрок не "приближался" 
  //к граням SkyBox в игровом пространстве.
  D3DXMATRIXA16 Mat_Trans,Mat_Rotate,Mat_Pos,Mat_Centered; 
  //                                        W           H           D
  D3DXMatrixTranslation(&Mat_Centered,-m_pWidth/2,-m_pHeight/2,-m_pDepth/2); 
  D3DXMatrixRotationYawPitchRoll(&Mat_Rotate,D3DX_PI,0.0f,0.0f);
  D3DXMatrixTranslation(&Mat_Pos,gr_Camera->Pos().x,gr_Camera->Pos().y,gr_Camera->Pos().z);
  Mat_Trans = (Mat_Centered*Mat_Rotate)*Mat_Pos; //Multiply Matrices
  d_pDevice->SetTransform(D3DTS_WORLD,&Mat_Trans); //Transform Object Position In Space

  //Draw The SkyBox Here
  //Set Vertex And Index Buffers To Render
  //Ну а здесь уже собственно происходит отрисовка самих граней (полигонов) 
  //SkyBox-куба и наложение текстур..
  d_pDevice->SetTexture(0,NULL);
 
  //Устанавливаем в устройстве текущий вершинный буфер
  d_pDevice->SetStreamSource(0,m_pVertexBuffer,0,sizeof(SKYBOX_VERTEX));
  //Устанавливаем подготовленный ранее FVF наших вершин D3DFVF_SKYBOXVERTEX)
  d_pDevice->SetFVF(D3DFVF_SKYBOXVERTEX); 
  //Устанавливаем индексы из ранее подготовленного индексного буфера:
  d_pDevice->SetIndices(m_pIndexBuffer);
  
  //Output For Created Objects
  //Рендеринг "стенок" куба и  текстур. Вывод примитивов 
  //посредством индексного буфера осуществляется методом DrawIndexedPrimitive

  //Render BackWall
  if (m_pTex[0] != NULL) d_pDevice->SetTexture(0,m_pTex[0]);
  d_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2 );
  
  //Render LeftWall
  if (m_pTex[1] != NULL) d_pDevice->SetTexture(0,m_pTex[1]);
  d_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 4, 0, 4, 0, 2 ); //Offset To 4 Base Vertex Index 

  //Render FrontWall
  if (m_pTex[2] != NULL) d_pDevice->SetTexture(0,m_pTex[2]);
  d_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 8, 0, 4, 0, 2 ); //Offset To 8 Base Vertex Index 

  //Render RightWall
  if (m_pTex[3] != NULL) d_pDevice->SetTexture(0,m_pTex[3]);
  d_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 12, 0, 4, 0, 2 ); //Offset To 12 Base Vertex Index 

  //Render Floor
  if (m_pTex[4] != NULL) d_pDevice->SetTexture(0,m_pTex[4]);
  d_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 16, 0, 4, 0, 2 ); //Offset To 16 Base Vertex Index

  //Render Ceiling
  if (m_pTex[5] != NULL) d_pDevice->SetTexture(0,m_pTex[5]);
  d_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 20, 0, 4, 0, 2 ); //Offset To 20 Base Vertex Index

  //Здесь уже возвращаем исходные параметры и активируем Z-Buffer, это необходимо
  //для устранения влияния параметров SkyBox на отрисовку последующих
  //элементов сцены которые будут рендериться после самого SkyBox-а в дальнейшем..
  d_pDevice->SetRenderState(D3DRS_ZENABLE,TRUE);
  d_pDevice->SetRenderState(D3DRS_ZWRITEENABLE,TRUE);
  
  //VU Addr Возвращаем исходные параметры для отрисовки текстур,
  //так называемые текстурные "сэмплеры" и устанавливаем их в режим 
  //"обёртки" - WRAP, если этого не сделать, то модель или другая геометрия
  //игрового "мира" имеющая текстуры будет рендериться некорректно, т.е 
  //изображение текстуры будет как бы растягиваться от краёв к центру, иными словами
  //будет сказываться влияние текстурного состояния CLAMP которое мы устанавливали
  //ранее для отрисовки SkyBox..
  d_pDevice->SetSamplerState(0,D3DSAMP_ADDRESSU,D3DTADDRESS_WRAP);
  d_pDevice->SetSamplerState(0,D3DSAMP_ADDRESSV,D3DTADDRESS_WRAP);
} //EndRenderProcedure

//-----------------------------------------------------------------------------