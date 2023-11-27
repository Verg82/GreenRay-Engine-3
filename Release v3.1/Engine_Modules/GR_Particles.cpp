/*******************************************************************************
// It's Unofficial Version Of The GreenRay Engine v.3.0.0.2
// GR_Particles.cpp
// It's The GreenRay Physics Engine.
// Made Specially For Implementing Particle System Technology.
// Date Creation: 29 August 2005
// Last Changing: 15 March 2009
// Licensed By: (C)DDD Digital Dreams Development And (C)3D Studios Inc., 2009
*******************************************************************************/
#include <GR_Particles.h>

//////////////////////////////////////////////////////////////////////////////
// Construction/Destruction For CGRParticle Class
//////////////////////////////////////////////////////////////////////////////
CGRParticle::CGRParticle(LPDIRECT3DDEVICE9 pDevice)
{
  d_pDevice = pDevice; //Apropritate Device Pointer To Class

  m_dwVBOffset       = 0;    //Gives the offset of the vertex buffer chunk that's currently being filled
  m_dwFlush          = 512;  //Number of point sprites to load before sending them to hardware(512 = 2048 divided into 4 chunks)
  m_dwDiscard        = 2048; //Max number of point sprites the vertex buffer can load until we are forced to discard and start over
  m_pActiveList      = NULL; //Head node of point sprites that are active
  m_pFreeList        = NULL; //Head node of point sprites that are inactive and waiting to be recycled.
  m_pPlanes          = NULL;
  m_dwActiveCount    = 0;
  m_fCurrentTime     = 0.0f;
  m_fLastUpdate      = 0.0f;
  m_pVB              = NULL; //The vertex buffer where point sprites are to be stored
  m_chTexFile        = NULL;
  m_pTexParticle     = NULL;
  m_dwMaxParticles   = 1;
  m_dwNumToRelease   = 1;
  m_fReleaseInterval = 1.0f;
  m_fLifeCycle       = 1.0f;
  m_fSize            = 1.0f;
  m_clrColor         = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
  m_vPosition        = D3DXVECTOR3(0.0f,0.0f,0.0f);
  m_vVelocity        = D3DXVECTOR3(0.0f,0.0f,0.0f);
  m_vGravity         = D3DXVECTOR3(0.0f,0.0f,0.0f);
  m_vWind            = D3DXVECTOR3(0.0f,0.0f,0.0f);
  m_bAirResistence   = true;
  m_fVelocityVar     = 1.0f;
  Is_Updated         = false;

  SetTexture("Rock.bmp");
} //EndConstructionDirectives

CGRParticle::~CGRParticle()
{
} //EndDestructionDirectives

//-----------------------------------------------------------------------------
// Name: Release()
// Desc: 
//-----------------------------------------------------------------------------
void CGRParticle::Release()
{
  InvalidateDeviceObjects();

  while (m_pPlanes) //Repeat Till NULL...
  {
    Plane* pPlane = m_pPlanes; //Hold Onto The First One
    m_pPlanes = pPlane->m_pNext; //Move Down To The Next One
    delete pPlane; //Delete The One We're Holding
  }

  while (m_pActiveList)
  {
    Particle* pParticle = m_pActiveList;
    m_pActiveList = pParticle->m_pNext;
    delete pParticle;
  }
  m_pActiveList = NULL;

  while (m_pFreeList)
  {
    Particle* pParticle = m_pFreeList;
    m_pFreeList = pParticle->m_pNext;
    delete pParticle;
  }
  m_pFreeList = NULL;

  if (m_chTexFile != NULL)
  {
    delete [] m_chTexFile;
	m_chTexFile = NULL;
  }

  if (m_pTexParticle != NULL) 
  {
    m_pTexParticle->Release();
    m_pTexParticle = NULL;
  }
} //EndReleaseProcedure

//-----------------------------------------------------------------------------
// Name: CGRParticle::ClassifyPoint()
// Desc: Classifies A Point Against The Plane Passed
//-----------------------------------------------------------------------------
int CGRParticle::ClassifyPoint(D3DXVECTOR3* vPoint,Plane* pPlane)
{
  D3DXVECTOR3 vDirection = pPlane->m_vPoint-*vPoint;
  float fResult = D3DXVec3Dot(&vDirection,&pPlane->m_vNormal);

  if (fResult < -0.001f) return CP_FRONT;
  if (fResult > 0.001f) return CP_BACK;

  return CP_ONPLANE;
} //EndClassifyPointFunction

//-----------------------------------------------------------------------------
// Name: CGRParticle::GetRandomMinMax()
// Desc: Gets a random number between min/max boundaries
//-----------------------------------------------------------------------------
float CGRParticle::GetRandomMinMax(float fMin,float fMax)
{
  float fRandNum = (float)rand()/RAND_MAX;
  return fMin+(fMax-fMin)*fRandNum;
} //EndGetRandomMinMaxFunction

//-----------------------------------------------------------------------------
// Name: CGRParticle::GetRandomVector()
// Desc: Generates A Random Vector Where X,Y, And Z Components Are Between 
// -1.0 and 1.0
//-----------------------------------------------------------------------------
D3DXVECTOR3 CGRParticle::GetRandomVector()
{
  D3DXVECTOR3 vVector;

  //Pick A Random Z Between -1.0f And 1.0f.
  vVector.z = GetRandomMinMax(-1.0f,1.0f);
    
  //Get Radius Of This Circle
  float Radius = (float)sqrt(1-vVector.z*vVector.z);
    
  //Pick A Random Point On A Circle.
  float t = GetRandomMinMax(-D3DX_PI,D3DX_PI);

  //Compute matching X and Y for our Z.
  vVector.x = (float)cosf(t)*Radius;
  vVector.y = (float)sinf(t)*Radius;

  return vVector;
} //EndGetRandomVectorFunction

//-----------------------------------------------------------------------------
// Name: CGRParticle::SetTexture()
// Desc: 
//-----------------------------------------------------------------------------
void CGRParticle::SetTexture(char* chTexFile)
{
  //Deallocate The Memory That Was Previously Reserved For This String
  if (m_chTexFile != NULL)
  {
    delete[] m_chTexFile;
	m_chTexFile = NULL;
  }
    
  //Dynamically Allocate The Correct Amount Of Memory.
  m_chTexFile = new char[strlen(chTexFile)+1];

  //If The Allocation Succeeds, Copy The Initialization String.
  if (m_chTexFile != NULL) strcpy(m_chTexFile,chTexFile);
} //EndSetTextureProcedure

//-----------------------------------------------------------------------------
// Name: CGRParticle::GetTexture()
// Desc: 
//-----------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 &CGRParticle::GetTexture()
{
  return m_pTexParticle;
} //EndGetTextureFunction

//-----------------------------------------------------------------------------
// Name: CGRParticle::SetCollisionPlane()
// Desc: 
//-----------------------------------------------------------------------------
void CGRParticle::SetCollisionPlane(D3DXVECTOR3 vPlaneNormal,D3DXVECTOR3 vPoint, 
float fBounceFactor,int nCollisionResult)
{
  Plane* pPlane = new Plane;
  pPlane->m_vNormal = vPlaneNormal;
  pPlane->m_vPoint = vPoint;
  pPlane->m_fBounceFactor = fBounceFactor;
  pPlane->m_nCollisionResult = nCollisionResult;

  pPlane->m_pNext = m_pPlanes; //Attach The Current List To It...
  m_pPlanes = pPlane; //And Make It The New Head.
} //EndSetCollisionPlaneProcedure

//-----------------------------------------------------------------------------
// Name: CGRParticle::Init()
// Desc: 
//-----------------------------------------------------------------------------
HRESULT CGRParticle::Init()
{
  HRESULT Result;

  //Initialize The Particle System
  if (FAILED(Result = RestoreDeviceObjects())) return Result;

  //Get Max Point Size
  D3DCAPS9 D3D_Caps;
  d_pDevice->GetDeviceCaps(&D3D_Caps);
  m_fMaxPointSize = D3D_Caps.MaxPointSize;

  //Check And See If We Can Change The Size Of Point Sprites 
  //In Hardware By Sending D3DFVF_PSIZE With The FVF.

  if (D3D_Caps.FVFCaps & D3DFVFCAPS_PSIZE) m_bDeviceSupportsPSIZE = true;
  else m_bDeviceSupportsPSIZE = false;

  //Load Texture Map For Particles
  //if (FAILED(D3DXCreateTextureFromFile(d_pDevice,m_chTexFile,&m_ptexParticle)))
  if (FAILED(D3DXCreateTextureFromFileEx(d_pDevice,m_chTexFile,0,0,0,0,
  D3DFMT_UNKNOWN,D3DPOOL_MANAGED,D3DX_DEFAULT,D3DX_DEFAULT,D3DCOLOR_XRGB(0,0,0),NULL,NULL,&m_pTexParticle)))
  {
    return Result;
  }
    return S_OK;
} //EndInitFunction

//-----------------------------------------------------------------------------
// Name: CGRParticle::Update()
// Desc:
//-----------------------------------------------------------------------------
HRESULT CGRParticle::Update()
{
  Particle* pParticle;
  Particle** ppParticle;
  Plane* pPlane;
  Plane** ppPlane;
  D3DXVECTOR3 vOldPosition;

  if (GetLifeCycle() > 0)
  {
    m_fCurrentTime+= gr_Timer->GetTime(); //Update System Timer For Our Particles...

    ppParticle = &m_pActiveList; //Start At The Head Of The Active List

    while (*ppParticle)
    {
       pParticle = *ppParticle; //Set A Pointer To The Head

       //Calculate New Position
       m_fTimePassed = m_fCurrentTime-pParticle->m_fInitTime;

       if (m_fTimePassed >= m_fLifeCycle)
       {
         //Time Is Up, Put The Particle Back On The Free List...
         *ppParticle = pParticle->m_pNext;
         pParticle->m_pNext = m_pFreeList;
         m_pFreeList = pParticle;

         --m_dwActiveCount;
	     Is_Updated = true; //Particles Is Updated Then Try To Release
       }
       else
       {
         //Update Particle Position And Velocity
         //Update Velocity With Respect To Gravity (Constant Acceleration)
         pParticle->m_vCurVel+= m_vGravity*gr_Timer->GetTime();

         //Update Velocity With Respect To Wind (Acceleration Based On Difference Of Vectors)
         if (m_bAirResistence == true) pParticle->m_vCurVel+= (m_vWind-pParticle->m_vCurVel)*gr_Timer->GetTime();

         //Finally, Update Position With Respect To Velocity
         vOldPosition = pParticle->m_vCurPos;
         pParticle->m_vCurPos+= pParticle->m_vCurVel*gr_Timer->GetTime();

         //Begin Checking The Particle Against Each Plane That Was Set Up
         ppPlane = &m_pPlanes; //Set A Pointer To The Head

         while (*ppPlane)
         {
           pPlane = *ppPlane;
           int Result = ClassifyPoint(&pParticle->m_vCurPos,pPlane);

           if (Result == CP_BACK /*||Result == CP_ONPLANE */)
           {
             if (pPlane->m_nCollisionResult == CR_BOUNCE)
             {
               pParticle->m_vCurPos = vOldPosition;
               //-----------------------------------------------------------------
               //
               //The new velocity vector of a particle that is bouncing off a plane is computed as follows:
               //
               //Vn = (N.V)*N
               //Vt = V-Vn
               //Vp = Vt-Kr*Vn
               //
               //Where:
               // 
               //.  = Dot product operation
               //N  = The normal of the plane from which we bounced
               //V  = Velocity vector prior to bounce
               //Vn = Normal force
               //Kr = The coefficient of restitution (Ex. 1 = Full Bounce, 0 = Particle Sticks)
               //Vp = New velocity vector after bounce
               //
               //-----------------------------------------------------------------

               float Kr = pPlane->m_fBounceFactor;

               D3DXVECTOR3 Vn = D3DXVec3Dot(&pPlane->m_vNormal,&pParticle->m_vCurVel)*pPlane->m_vNormal;
               D3DXVECTOR3 Vt = pParticle->m_vCurVel-Vn;
               D3DXVECTOR3 Vp = Vt-Kr*Vn;

               pParticle->m_vCurVel = Vp;
             }
             else if (pPlane->m_nCollisionResult == CR_RECYCLE)
             {
               pParticle->m_fInitTime-= m_fLifeCycle;
             }
             else if (pPlane->m_nCollisionResult == CR_STICK)
             {
               pParticle->m_vCurPos = vOldPosition;
               pParticle->m_vCurVel = D3DXVECTOR3(0.0f,0.0f,0.0f);
             }
           }
           ppPlane = &pPlane->m_pNext;
         }

         //End Plane Checking
         //-----------------------------------------------------------------
         ppParticle = &pParticle->m_pNext;
       }
     }

    //-------------------------------------------------------------------------
    //Emit new particles in accordance to the flow rate...
    // 
    //NOTE: The system operates with a finite number of particles.
    //      New particles will be created until the max amount has
    //      been reached, after that, only old particles that have
    //      been recycled can be reintialized and used again.
    //-------------------------------------------------------------------------
    if (m_fCurrentTime-m_fLastUpdate > m_fReleaseInterval)
    {
      //Reset Update Timing...
      m_fLastUpdate = m_fCurrentTime;
    
      //Emit new particles at specified flow rate...
      for (DWORD I = 0; I < m_dwNumToRelease; ++I)
      {
        //Do We Have Any Free Particles To Put Back To Work?
        if (m_pFreeList)
        {
          //If So, Hand Over The First Free One To Be Reused.
          pParticle = m_pFreeList;
          //Then Make The Next Free Particle In The List Next To Go!
          m_pFreeList = pParticle->m_pNext;
        }
        else
        {
          //There Are No Free Particles To Recycle...
          //We'll Have To Create A New One From Scratch!
          if (m_dwActiveCount < m_dwMaxParticles)
          {
            if (NULL == (pParticle = new Particle)) return E_OUTOFMEMORY;
          }
        }

        if (m_dwActiveCount < m_dwMaxParticles)
        {
          pParticle->m_pNext = m_pActiveList; //Make It The New Head...
          m_pActiveList = pParticle;
                
          //Set The Attributes For Our New Particle...
          pParticle->m_vCurVel = m_vVelocity;

          if (m_fVelocityVar != 0.0f)
          {
            D3DXVECTOR3 vRandomVec = GetRandomVector();
            pParticle->m_vCurVel+= vRandomVec*m_fVelocityVar;
          }
          pParticle->m_fInitTime = m_fCurrentTime;
          pParticle->m_vCurPos = m_vPosition;      
          ++m_dwActiveCount;
        }
      }
    }
  }
  return S_OK;
} //EndUpdateFunction

//-----------------------------------------------------------------------------
// Name: CGRParticle::RestartParticleSystem()
// Desc:
//-----------------------------------------------------------------------------
void CGRParticle::RestartParticleSystem()
{
  Particle* pParticle;
  Particle** ppParticle;
  ppParticle = &m_pActiveList; //Start At The Head Of The Active List

  while (*ppParticle)
  {
    pParticle = *ppParticle; //Set A Pointer To The Head

    //Put The Particle Back On The Free List...
	*ppParticle = pParticle->m_pNext;
	pParticle->m_pNext = m_pFreeList;
	m_pFreeList = pParticle;

	--m_dwActiveCount;
  }
} //EndRestartParticleSystemProcedure

//-----------------------------------------------------------------------------
// Name: CGRParticle::Render()
// Desc: Renders the particle system using pointsprites loaded in a vertex 
//       buffer.
//
// Note: D3DLOCK_DISCARD:
//
//       The application overwrites, with a write-only operation, the entire 
//       index buffer. This enables Direct3D to return a pointer to a new 
//       memory area so that the dynamic memory access (DMA) and rendering 
//       from the old area do not stall.
//
//       D3DLOCK_NOOVERWRITE:
//
//       Indicates that no vertices that were referred to in drawing calls 
//       since the start of the frame or the last lock without this flag will 
//       be modified during the lock. This can enable optimizations when the 
//       application is appending data only to the vertex buffer. 
//-----------------------------------------------------------------------------
HRESULT CGRParticle::Render()
{
  HRESULT Result;

  //Set The Render States For Using Point Sprites..
  d_pDevice->SetRenderState(D3DRS_POINTSPRITEENABLE,TRUE); //Turn On Point Sprites
  d_pDevice->SetRenderState(D3DRS_POINTSCALEENABLE,TRUE); //Allow Sprites To Be Scaled With Distance
  //Float Value That Specifies The Size To Use For Point Size Computation 
  //In Cases Where Point Size Is Not Specified For Each Vertex.
  d_pDevice->SetRenderState(D3DRS_POINTSIZE,FloatToDWord(m_fSize)); 
  //Float Value That Specifies The Minimum Size Of Point Primitives. 
  //Point Primitives Are Clamped To This Size During Rendering.
  d_pDevice->SetRenderState(D3DRS_POINTSIZE_MIN,FloatToDWord(1.0f));  
  d_pDevice->SetRenderState(D3DRS_POINTSCALE_A,FloatToDWord(0.0f)); //Default 1.0
  d_pDevice->SetRenderState(D3DRS_POINTSCALE_B,FloatToDWord(0.0f)); //Default 0.0
  d_pDevice->SetRenderState(D3DRS_POINTSCALE_C,FloatToDWord(1.0f)); //Default 0.0

  Particle* pParticle = m_pActiveList;
  PointVertex* pVertices;
  DWORD dwNumParticlesToRender = 0;

  //Lock The Vertex Buffer. We Fill The Vertex Buffer In Small
  //Chunks, Using D3DLOCK_NOOVERWRITE. When We Are Done Filling
  //Each Chunk, We Call DrawPrim, And Lock The Next Chunk. When
  //We Run Out Of Space In The Vertex Buffer, We Start Over At
  //The Beginning, Using D3DLOCK_DISCARD.

  //Move The Offset Forward So We Can Fill The Next Chunk Of The Vertex Buffer
  m_dwVBOffset+= m_dwFlush;

  //If We're About To Overflow The Buffer, Reset The Offset Counter Back To 0
  if (m_dwVBOffset >= m_dwDiscard) m_dwVBOffset = 0;

  if (FAILED(Result = m_pVB->Lock(m_dwVBOffset*sizeof(PointVertex), //Offset To Lock
  m_dwFlush*sizeof(PointVertex), //Size To Lock
  (void**)&pVertices,m_dwVBOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD)))
  {
    return Result;
  }

  //Render Each Particle
  while (pParticle)
  {
    D3DXVECTOR3 vPos(pParticle->m_vCurPos);
    D3DXVECTOR3 vVel(pParticle->m_vCurVel);

    pVertices->posit = vPos;
    pVertices->color = m_clrColor;
    pVertices++;

    if (++dwNumParticlesToRender == m_dwFlush)
    {
      //Done Filling This Chunk Of The Vertex Buffer. Lets Unlock And
      //Draw This Portion So We Can Begin Filling The Next Chunk.

      m_pVB->Unlock();

	  d_pDevice->SetStreamSource(0,m_pVB,0,sizeof(PointVertex));
	  d_pDevice->SetFVF(PointVertex::FVF_Flags);

      if (FAILED(Result = d_pDevice->DrawPrimitive(D3DPT_POINTLIST,m_dwVBOffset,dwNumParticlesToRender)))
      {
        return Result;
      }

      //Lock The Next Chunk Of The Vertex Buffer. If We Are At The 
      //End Of The Vertex Buffer, DISCARD The Vertex Buffer And Start
      //At The Beginning. Otherwise, Specify NOOVERWRITE, So We Can
      //Continue Filling The VB While The Previous Chunk Is Drawing.
      m_dwVBOffset+= m_dwFlush;

      //If We're About To Overflow The Buffer, Reset The Offset Counter Back To 0
      if (m_dwVBOffset >= m_dwDiscard) m_dwVBOffset = 0;

      if (FAILED(Result = m_pVB->Lock(m_dwVBOffset*sizeof(PointVertex), //Offset To Lock
      m_dwFlush*sizeof(PointVertex), //Size To Lock
      (void**)&pVertices,m_dwVBOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD)))
      {
        return Result;
      }

      dwNumParticlesToRender = 0;
    }
    pParticle = pParticle->m_pNext;
  }

  //Unlock The Vertex Buffer
  m_pVB->Unlock();

  //Render Any Remaining Particles
  if (dwNumParticlesToRender)
  {
    d_pDevice->SetStreamSource(0,m_pVB,0,sizeof(PointVertex));
	d_pDevice->SetFVF(PointVertex::FVF_Flags);

    if (FAILED(Result = d_pDevice->DrawPrimitive(D3DPT_POINTLIST,m_dwVBOffset,dwNumParticlesToRender)))
	{
	  return Result;
	}
  }

  //Reset Render States...
  d_pDevice->SetRenderState(D3DRS_POINTSPRITEENABLE,FALSE);
  d_pDevice->SetRenderState(D3DRS_POINTSCALEENABLE,FALSE);

  return S_OK;
} //EndRenderFunction

//-----------------------------------------------------------------------------
// Name: CGRParticle::RestoreDeviceObjects()
// Desc:
// Note: If your application performs its own vertex processing and passes 
//       transformed, lit, and clipped vertices to rendering methods, then the 
//       application can directly write vertices to a vertex buffer allocated  
//       in driver-optimal memory. This technique prevents a redundant copy  
//       operation later. Note that this technique will not work well if your 
//       application reads data back from a vertex buffer, because read 
//       operations done by the host from driver-optimal memory can be very 
//       slow. Therefore, if your application needs to read during processing 
//       or writes data to the buffer erratically, a system-memory vertex 
//       buffer is a better choice.
//
//       When using the Direct3D vertex processing features (by passing 
//       untransformed vertices to vertex-buffer rendering methods), processing 
//       can occur in either hardware or software depending on the device type  
//       and device creation flags. It is recommended that vertex buffers be 
//       allocated in pool D3DPOOL_DEFAULT for best performance in virtually 
//       all cases. When a device is using hardware vertex processing, there 
//       is a number of additional optimizations that may be done based on the 
///      flags  D3DUSAGE_DYNAMIC and D3DUSAGE_WRITEONLY. 
//       See IDirect3DDevice9::CreateVertexBuffer for more information on 
//       using these flags.
//-----------------------------------------------------------------------------
HRESULT CGRParticle::RestoreDeviceObjects()
{
  HRESULT Result;

  //Create A Vertex Buffer For The Particle System. The Size Of This Buffer
  //Does Not Relate To The Number Of Particles That Exist. Rather, The
  //Buffer Is Used As A Communication Channel With The Device. We Fill In 
  //A Chunck, And Tell The Device To draw. While The Device Is Drawing, We
  //Fill In The Next Chunck Using NOOVERWRITE. We Continue Doing This Until 
  //We Run Out Of Vertex Buffer Space, And Are Forced To DISCARD The Buffer
  //And Start Over At The Beginning.

  if (FAILED(Result = d_pDevice->CreateVertexBuffer(m_dwDiscard*sizeof(PointVertex), 
  D3DUSAGE_DYNAMIC|D3DUSAGE_WRITEONLY|D3DUSAGE_POINTS,PointVertex::FVF_Flags, //Our Custom FVF
  D3DPOOL_DEFAULT,&m_pVB,NULL)))
  {
    return E_FAIL;
  }

  return S_OK;
} //EndRestoreDeviceObjectsFunction

//-----------------------------------------------------------------------------
// Name: CGRParticle::InvalidateDeviceObjects()
// Desc:
//-----------------------------------------------------------------------------
HRESULT CGRParticle::InvalidateDeviceObjects()
{
  if (m_pVB != NULL) m_pVB->Release();
  return S_OK;
} //EndInvalidateDeviceObjectsFunction

//-----------------------------------------------------------------------------

