/*******************************************************************************
// It's Unofficial Version Of The GreenRay Engine v.3.0.0.2
// GR_Models.cpp
// It's The GreenRay Model Loader Source File.
// Made Specially For Implementing Loading And Rendering Mesh Files System.
// Programming By: Verg
// Date Creation: 29 August 2005
// Last Changing: 24 August 2007
// Author Rights By: Zie Technology And Lucky's Flash Studio Inc.,
// Licensed By: (C)DDD Digital Dreams Development And (C)3D Studios Inc., 2007
*******************************************************************************/
#include <GR_Models.h>
#include <GRMethods.h>

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction For CGRModel Class
///////////////////////////////////////////////////////////////////////////////
CGRModel::CGRModel()
{  
  m_pSysMemMesh = NULL;
  m_pLocalMesh = NULL;
  m_dwNumMaterials = 0L;
  m_pMaterials = NULL;
  m_pTextures = NULL;
  m_bUseMaterials = TRUE;
} //EndConstructionDirectives

CGRModel::~CGRModel()
{
  Destroy();
} //EndDestructionDirectives

//-----------------------------------------------------------------------------
// Name: CGRModel::Destroy()
// Desc:
//-----------------------------------------------------------------------------
HRESULT CGRModel::Destroy()
{
  InvalidateObjects();
  for (UINT i = 0; i < m_dwNumMaterials; i++) ReleaseClass(m_pTextures[i]);
  DeleteArray(m_pTextures);
  DeleteArray(m_pMaterials);

  ReleaseClass(m_pSysMemMesh);
  m_dwNumMaterials = NULL;

  return S_OK;
} //EndDestroyFunction

//-----------------------------------------------------------------------------
// Name: CGRModel::Create()
// Desc:
//-----------------------------------------------------------------------------
HRESULT CGRModel::Create(LPDIRECT3DDEVICE9 pDevice,LPCWSTR File_Name)
{
  d_pDevice = pDevice; //Apropritate Device Pointer To Class
  WCHAR StrPath[MAX_PATH];
  LPD3DXBUFFER pAdjacencyBuffer = NULL;
  LPD3DXBUFFER pMtrlBuffer = NULL;
  HRESULT Result;

  //Load The Mesh
  Result = D3DXLoadMeshFromXW(File_Name,D3DXMESH_SYSTEMMEM,d_pDevice, 
  &pAdjacencyBuffer,&pMtrlBuffer,NULL,&m_dwNumMaterials,&m_pSysMemMesh);

  //Optimize The Mesh For Performance
  if (FAILED(Result = m_pSysMemMesh->OptimizeInplace(
  D3DXMESHOPT_COMPACT|D3DXMESHOPT_ATTRSORT|D3DXMESHOPT_VERTEXCACHE,
  (DWORD*)pAdjacencyBuffer->GetBufferPointer(),NULL,NULL,NULL)))
  {
    ReleaseClass(pAdjacencyBuffer);
    ReleaseClass(pMtrlBuffer);
    return Result;
  }

  //Set StrPath To The Path Of The Mesh File
  WCHAR* pLastBSlash = wcsrchr(StrPath,L'\\');
  if (pLastBSlash) *(pLastBSlash+1) = L'\0';
  else *StrPath = L'\0';

  Result = CreateMaterials(StrPath,pAdjacencyBuffer,pMtrlBuffer);

  ReleaseClass(pAdjacencyBuffer);
  ReleaseClass(pMtrlBuffer);

  return Result;
} //EndCreateFunction

//-----------------------------------------------------------------------------
// Name: CGRModel::CreateMaterials()
// Desc:
//-----------------------------------------------------------------------------
HRESULT CGRModel::CreateMaterials(LPCWSTR Str_Path,
ID3DXBuffer* pAdjacencyBuffer,ID3DXBuffer* pMtrlBuffer)
{
  char* Texture_Name; //This Value Keep Our Texture Name

  //Get Material Info For The Mesh
  //Get The Array Of Materials Out Of The Buffer
  if (pMtrlBuffer && m_dwNumMaterials > 0)
  {
    //Allocate Memory For The Materials And Textures
    D3DXMATERIAL* d3dxMtrls = (D3DXMATERIAL*)pMtrlBuffer->GetBufferPointer();
    m_pMaterials = new D3DMATERIAL9[m_dwNumMaterials];
    if (m_pMaterials == NULL) return E_OUTOFMEMORY;

    //m_pTextures = new LPDIRECT3DBASETEXTURE9[m_dwNumMaterials];
    m_pTextures = new LPDIRECT3DTEXTURE9[m_dwNumMaterials];
    if (m_pTextures == NULL) return E_OUTOFMEMORY;

    //Copy Each Material And Create It's Texture
    for (DWORD I = 0; I < m_dwNumMaterials; I++)
    {
      //Copy The Material
      m_pMaterials[I] = d3dxMtrls[I].MatD3D;
      m_pTextures[I] = NULL;
  
	  //Create A Texture
	  if (d3dxMtrls[I].pTextureFilename)
      {
        Texture_Name = NULL;
        Texture_Name = new char[MAX_PATH];
        strcpy(Texture_Name,m_pTexPath);
        strcat(Texture_Name,d3dxMtrls[I].pTextureFilename);

	    //Create And Load Texture
        if (FAILED(D3DXCreateTextureFromFile(d_pDevice,Texture_Name,&m_pTextures[I])))
	    {
          m_pTextures[I] = NULL;
	    }
	  }
	}
/*
  //Create A Texture
  if (d3dxMtrls[i].pTextureFilename)
  {
    WCHAR strTexture[MAX_PATH];
    WCHAR strTextureTemp[MAX_PATH];
    D3DXIMAGE_INFO ImgInfo;

    //First Attempt To Look For Texture In The Same Folder As The Input Folder.
    MultiByteToWideChar(CP_ACP,0,d3dxMtrls[i].pTextureFilename,-1,strTextureTemp,MAX_PATH);
    strTextureTemp[MAX_PATH-1] = 0;

    StringCchCopy((char*)strTexture,MAX_PATH,(const char*)strPath);
    StringCchCat((char*)strTexture,MAX_PATH,(const char*)strTextureTemp);

    //Inspect The Texture File To Determine The Texture Type.
    if (FAILED(D3DXGetImageInfoFromFile(LPCSTR(strTexture),&ImgInfo)))
    {
      //Search The Media Folder
      if (FAILED(DXUTFindDXSDKMediaFileCch(strTexture,MAX_PATH,strTextureTemp)))
      continue; //Can't Find. Skip.

      D3DXGetImageInfoFromFile(LPCSTR(strTexture),&ImgInfo);
    }

    //Call The Appropriate Loader According To The Texture Type.
    switch (ImgInfo.ResourceType)
    {
      case D3DRTYPE_TEXTURE:
      {
        IDirect3DTexture9* pTex;
        if (SUCCEEDED(D3DXCreateTextureFromFile(pd3dDevice,LPCSTR(strTexture),&pTex)))
        {
          //Obtain The Base Texture Interface
          pTex->QueryInterface(IID_IDirect3DBaseTexture9,(LPVOID*)&m_pTextures[i]);
          //Release The Specialized Instance
          pTex->Release();
        }
        break;
      }
      
	  case D3DRTYPE_CUBETEXTURE:
      {
        IDirect3DCubeTexture9* pTex;
        if (SUCCEEDED(D3DXCreateCubeTextureFromFile(pd3dDevice,LPCSTR(strTexture),&pTex)))
        {
          //Obtain The Base Texture Interface
          pTex->QueryInterface(IID_IDirect3DBaseTexture9,(LPVOID*)&m_pTextures[i]);
          //Release The Specialized Instance
          pTex->Release();
        }
        break;
      }
      
	  case D3DRTYPE_VOLUMETEXTURE:
      {
        IDirect3DVolumeTexture9* pTex;
        if (SUCCEEDED(D3DXCreateVolumeTextureFromFile(pd3dDevice,LPCSTR(strTexture),&pTex)))
        {
          //Obtain The Base Texture Interface
          pTex->QueryInterface(IID_IDirect3DBaseTexture9,(LPVOID*)&m_pTextures[i]);
          //Release The Specialized Instance
          pTex->Release();
        }
        break;
      }
    }*/
  }
  delete Texture_Name;
  return S_OK;
} //EndCreateMaterialsFunction

//-----------------------------------------------------------------------------
// Name: CGRModel::SetTexturePath()
// Desc: This Procedure For Setting Path Where Texture There Found
//-----------------------------------------------------------------------------
void CGRModel::SetTexturePath(LPCSTR Tex_Path)
{
  m_pTexPath = Tex_Path;
} //EndSetTexturePathProcedure

//-----------------------------------------------------------------------------
// Name: CGRModel::SetFVF()
// Desc: Set Flexible Vertex Format
//-----------------------------------------------------------------------------
HRESULT CGRModel::SetFVF(DWORD dwFVF)
{
  LPD3DXMESH pTempSysMemMesh = NULL;
  LPD3DXMESH pTempLocalMesh = NULL;

  if (m_pSysMemMesh)
  {
    if (FAILED(m_pSysMemMesh->CloneMeshFVF(m_pSysMemMesh->GetOptions(),dwFVF,
	d_pDevice,&pTempSysMemMesh))) return E_FAIL;
  }
   
  if (m_pLocalMesh)
  {
    if (FAILED(m_pLocalMesh->CloneMeshFVF(m_pLocalMesh->GetOptions(),dwFVF,
    d_pDevice,&pTempLocalMesh)))
    {
      ReleaseClass(pTempSysMemMesh);
      return E_FAIL;
    }
  }

  DWORD dwOldFVF = 0;

  if (m_pSysMemMesh) dwOldFVF = m_pSysMemMesh->GetFVF();

  ReleaseClass(m_pSysMemMesh);
  ReleaseClass(m_pLocalMesh);

  if (pTempSysMemMesh) m_pSysMemMesh = pTempSysMemMesh;
  if (pTempLocalMesh) m_pLocalMesh = pTempLocalMesh;

  //Compute Normals If They Are Being Requested And The Old Mesh Does Not Have Them.
  if (!(dwOldFVF & D3DFVF_NORMAL) && dwFVF & D3DFVF_NORMAL)
  {
    if (m_pSysMemMesh) D3DXComputeNormals(m_pSysMemMesh,NULL);
    if (m_pLocalMesh) D3DXComputeNormals(m_pLocalMesh,NULL);
  }
  return S_OK;
} //EndSetFVFFunction

//-----------------------------------------------------------------------------
// Name: CGRModel::SetVertexDecl
// Desc: Convert The Mesh To The Format Specified By The Given Vertex Declarations
//-----------------------------------------------------------------------------
HRESULT CGRModel::SetVertexDecl(const D3DVERTEXELEMENT9* pDecl)
{
  LPD3DXMESH pTempSysMemMesh = NULL;
  LPD3DXMESH pTempLocalMesh = NULL;

  if (m_pSysMemMesh)
  {
    if (FAILED(m_pSysMemMesh->CloneMesh(m_pSysMemMesh->GetOptions(),pDecl,
    d_pDevice,&pTempSysMemMesh))) return E_FAIL;
  }

  if (m_pLocalMesh)
  {
    if (FAILED(m_pLocalMesh->CloneMesh(m_pLocalMesh->GetOptions(),pDecl,
    d_pDevice,&pTempLocalMesh)))
    {
      ReleaseClass(pTempSysMemMesh);
      return E_FAIL;
    }
  }
  
  //Check If The Old Declaration Contains A Normal.
  bool bHadNormal = false;
  D3DVERTEXELEMENT9 aOldDecl[MAX_FVF_DECL_SIZE];
  if (m_pSysMemMesh && SUCCEEDED(m_pSysMemMesh->GetDeclaration(aOldDecl)))
  {
    for (UINT index = 0; index < D3DXGetDeclLength(aOldDecl); ++index)
    if (aOldDecl[index].Usage == D3DDECLUSAGE_NORMAL)
    {
      bHadNormal = true;
      break;
    }
  }

  //Check If The New Declaration Contains A Normal.
  bool bHaveNormalNow = false;
  D3DVERTEXELEMENT9 aNewDecl[MAX_FVF_DECL_SIZE];
  if (pTempSysMemMesh && SUCCEEDED(pTempSysMemMesh->GetDeclaration(aNewDecl)))
  {
    for (UINT index = 0; index < D3DXGetDeclLength(aNewDecl); ++index)
    if (aNewDecl[index].Usage == D3DDECLUSAGE_NORMAL)
    {
      bHaveNormalNow = true;
      break;
    }
  }

  ReleaseClass(m_pSysMemMesh);
  ReleaseClass(m_pLocalMesh);

  if (pTempSysMemMesh)
  {
    m_pSysMemMesh = pTempSysMemMesh;

    if (!bHadNormal && bHaveNormalNow)
    {
      //Compute Normals In Case The Meshes Have Them
      D3DXComputeNormals(m_pSysMemMesh,NULL);
    }
  }

  if (pTempLocalMesh)
  {
    m_pLocalMesh = pTempLocalMesh;

    if (!bHadNormal && bHaveNormalNow)
    {
      //Compute Normals In Case The Meshes Have Them
      D3DXComputeNormals(m_pLocalMesh,NULL);
    }
  }

  return S_OK;
} //EndSetVertexDeclFunction

//-----------------------------------------------------------------------------
// Name: CGRModel::RestoreObjects()
// Desc:
//-----------------------------------------------------------------------------
HRESULT CGRModel::RestoreObjects()
{
  if (NULL == m_pSysMemMesh) return E_FAIL;

  //Make A Local Memory Version Of The Mesh. 
  //Note: Because We Are Passing In No flags, The Default 
  //Behavior Is To Clone Into Local Memory.
  if (FAILED(m_pSysMemMesh->CloneMeshFVF(
  D3DXMESH_MANAGED|(m_pSysMemMesh->GetOptions()&~D3DXMESH_SYSTEMMEM),
  m_pSysMemMesh->GetFVF(),d_pDevice,&m_pLocalMesh))) return E_FAIL;

  return S_OK;
} //EndRestoreObjectsFunction

//-----------------------------------------------------------------------------
// Name: CGRModel::InvalidateObjects()
// Desc:
//-----------------------------------------------------------------------------
HRESULT CGRModel::InvalidateObjects()
{
  ReleaseClass(m_pLocalMesh);
  return S_OK;
} //EndInvalidateObjectsFunction

//-----------------------------------------------------------------------------
// Name: CGRModel::Render1()
// Desc:
//-----------------------------------------------------------------------------
HRESULT CGRModel::Render(bool bDrawOpaqueSubsets,bool bDrawAlphaSubsets)
{
  if (NULL == m_pLocalMesh) return E_FAIL;

  //First, Draw The Subsets Without Alpha
  if (bDrawOpaqueSubsets)
  {
    for (DWORD i = 0; i < m_dwNumMaterials; i++)
    {
      if (m_bUseMaterials)
      {
        if (m_pMaterials[i].Diffuse.a < 1.0f) continue;
        d_pDevice->SetMaterial(&m_pMaterials[i]);
        d_pDevice->SetTexture(0,m_pTextures[i]);
      }
      m_pLocalMesh->DrawSubset(i);
    }
  }

  //Then, Draw The Subsets With Alpha
  if (bDrawAlphaSubsets && m_bUseMaterials)
  {
    for (DWORD i = 0; i < m_dwNumMaterials; i++)
    {
      if (m_pMaterials[i].Diffuse.a == 1.0f) continue;

      //Set The Material And Texture
      d_pDevice->SetMaterial(&m_pMaterials[i]);
      d_pDevice->SetTexture(0,m_pTextures[i]);
      m_pLocalMesh->DrawSubset(i);
    }
  }

  return S_OK;
} //EndRender1Function

//-----------------------------------------------------------------------------
// Name: CGRModel::Render2()
// Desc:
//-----------------------------------------------------------------------------
HRESULT CGRModel::Render(ID3DXEffect* pEffect,
D3DXHANDLE hTexture,D3DXHANDLE hDiffuse,
D3DXHANDLE hAmbient,D3DXHANDLE hSpecular,
D3DXHANDLE hEmissive,D3DXHANDLE hPower,
bool bDrawOpaqueSubsets,bool bDrawAlphaSubsets)
{
  if (NULL == m_pLocalMesh) return E_FAIL;

  UINT cPasses;
  //First, Draw The Subsets Without alpha
  if (bDrawOpaqueSubsets)
  {
    pEffect->Begin(&cPasses,0);
    for (UINT p = 0; p < cPasses; ++p)
    {
      pEffect->BeginPass(p);
      for (DWORD i = 0; i < m_dwNumMaterials; i++)
      {
        if (m_bUseMaterials)
        {
          if (m_pMaterials[i].Diffuse.a < 1.0f) continue;
          if (hTexture) pEffect->SetTexture(hTexture,m_pTextures[i]);
          //D3DCOLORVALUE And D3DXVECTOR4 Are Data-Wise Identical.
          //No Conversion Is Needed.
          if (hDiffuse) pEffect->SetVector(hDiffuse,(D3DXVECTOR4*)&m_pMaterials[i].Diffuse);
          if (hAmbient) pEffect->SetVector(hAmbient,(D3DXVECTOR4*)&m_pMaterials[i].Ambient);
          if (hSpecular) pEffect->SetVector(hSpecular,(D3DXVECTOR4*)&m_pMaterials[i].Specular);
          if (hEmissive) pEffect->SetVector(hEmissive,(D3DXVECTOR4*)&m_pMaterials[i].Emissive);
          if (hPower) pEffect->SetVector(hPower,(D3DXVECTOR4*)&m_pMaterials[i].Power);
          pEffect->CommitChanges();
        }
        m_pLocalMesh->DrawSubset(i);
      }
      pEffect->EndPass();
    }
    pEffect->End();
  }

  //Then, Draw The Subsets With Alpha
  if (bDrawAlphaSubsets && m_bUseMaterials)
  {
    pEffect->Begin(&cPasses,0);
    for (UINT p = 0; p < cPasses; ++p)
    {
      pEffect->BeginPass(p);
      for (DWORD i = 0; i < m_dwNumMaterials; i++)
      {
        if (m_bUseMaterials)
        {
          if (m_pMaterials[i].Diffuse.a == 1.0f) continue;
          if (hTexture) pEffect->SetTexture(hTexture,m_pTextures[i]);
          //D3DCOLORVALUE And D3DXVECTOR4 Are Data-Wise Identical.
          //No Conversion Is Needed.
          if (hDiffuse) pEffect->SetVector(hDiffuse,(D3DXVECTOR4*)&m_pMaterials[i].Diffuse);
          if (hAmbient) pEffect->SetVector(hAmbient,(D3DXVECTOR4*)&m_pMaterials[i].Ambient);
          if (hSpecular) pEffect->SetVector(hSpecular,(D3DXVECTOR4*)&m_pMaterials[i].Specular);
          if (hEmissive) pEffect->SetVector(hEmissive,(D3DXVECTOR4*)&m_pMaterials[i].Emissive);
          if (hPower) pEffect->SetVector(hPower,(D3DXVECTOR4*)&m_pMaterials[i].Power);
          pEffect->CommitChanges();
        }
        m_pLocalMesh->DrawSubset(i);
      }
      pEffect->EndPass();
    }
    pEffect->End();
  }

  return S_OK;
} //EndRender2Function

/*struct Vertex
{
Vertex(){}
Vertex(float x, float y, float z,
float nx, float ny, float nz, float u, float v)
{
 _x = x;   _y = y;   _z = z;
 _nx = nx; _ny = ny; _nz = nz;
 _u = u;   _v = v;
}

float _x, _y, _z, _nx, _ny, _nz, _u, _v;

static const DWORD FVF;
};

//-----------------------------------------------------------------------------
// Name: SaveMeshStructure()
// Desc: Saving Mesh Structure To A Text File
//-----------------------------------------------------------------------------
bool SaveMeshStructure(const char* File_Name)
{
  //Prepare AndLoad Mesh X-File Structure
  ID3DXMesh* m_pMesh = 0;
  ID3DXBuffer* MeshBuffer = 0;

  //Load The Mesh
  D3DXLoadMeshFromX("\Models\\Cube.x",D3DXMESH_MANAGED,m_pDevice,&MeshBuffer,0,0,0,&m_pMesh);

  m_pMesh->OptimizeInplace(D3DXMESHOPT_ATTRSORT|D3DXMESHOPT_COMPACT|D3DXMESHOPT_VERTEXCACHE, 
  (DWORD*)MeshBuffer->GetBufferPointer(),0,0,0);


  //Open File For Write Mode
  FILE *Inp_File = fopen(File_Name,"w"); 
  if (Inp_File == NULL) return false;
  int Block_Inc = 0;



  //Translation Matrices For The Object
  D3DXMATRIXA16 _MatTrans; 
  //                               X Y Z
  D3DXMatrixTranslation(&_MatTrans,0,0,0); 
 
 // D3DXMatrixTranslation(&_MatPos,0,0,0);

 // m_pDevice->SetTransform(D3DTS_WORLD,&_MatTrans); //Transform Object Position In Space


  LPDIRECT3DVERTEXBUFFER9 pVBuffer;
  Vertex* v = 0;
  //float* Vertex;
  DWORD Num_Vert = m_pMesh->GetNumVertices();
  DWORD Num_PerVertex = m_pMesh->GetNumBytesPerVertex();
  Num_PerVertex = Num_PerVertex/sizeof(float);
  D3DXVECTOR3 Vect;
  
  m_pMesh->GetVertexBuffer(&pVBuffer);
  pVBuffer->Lock(0,0,(void**)&v,D3DLOCK_READONLY);
  
  //Write D3DXMesh Data To File
  fprintf(Inp_File,"Begin PolyList\n");

  //Loop Through The Vertices
  for (DWORD I = 0; I < Num_Vert; I++) 
  {
	Block_Inc++;
 /* Vect.x = Vertex[I];
    Vect.y = Vertex[I+1];
    Vect.z = Vertex[I+2];
    
	//D3DXVec3TransformCoord(&Vect,&Vect,&_MatTrans);

    Vertex[I]   = Vect.x;
    Vertex[I+1] = Vect.y;
    Vertex[I+2] = Vect.z;   
*/
	/*if (Block_Inc == 1) 
	{
	  fprintf(Inp_File,"   Begin Polygon Texture=ArenaTex._ Flags=1073741824 Link=1\n");
	}
	
	//Write Vertex Positions In Space
    fprintf(Inp_File,"      Vertex   %0.02f,%0.02f,%0.02f\n",v[I]._x,v[I]._y,v[I]._z); 

	if (Block_Inc == 4) 
	{
      Block_Inc = 0;
	  fprintf(Inp_File,"   End Polygon\n");
	}
  }

  fprintf(Inp_File,"End PolyList");

  //Free All Resources
  fclose(Inp_File);
  pVBuffer->Unlock();
  pVBuffer->Release();
  
  MeshBuffer->Release(); MeshBuffer = NULL;
  m_pMesh->Release(); m_pMesh = NULL;

  return true;
}*/
/*

D3DXMATRIXA16 _ScaleMatrix;
D3DXMATRIXA16 _RollMatrix;
D3DXMATRIXA16 _WrldMatrix;
D3DXMATRIXA16 _ObjectToView; //Result Matrix An Getting Transform

//-----------------------------------------------------------------------------
// Name: CGRModels::RotationModel()
// Desc: 
//-----------------------------------------------------------------------------
void CGRModels::RotationModel(float X,float Y,float Z)
{
  D3DXMatrixRotationYawPitchRoll(&_RollMatrix,X,Y,Z);
} //EndRotationModelProcedure

//-----------------------------------------------------------------------------
// Name: CGRModels::ScaleModel()
// Desc: 
//-----------------------------------------------------------------------------
void CGRModels::ScaleModel(float X,float Y,float Z)
{
  D3DXMatrixScaling(&_ScaleMatrix,X,Y,Z);
} //EndScaleModelProcedure

//-----------------------------------------------------------------------------
// Name: CGRModels::DispositionModeln()
// Desc: 
//-----------------------------------------------------------------------------
void CGRModels::DispositionModel(float X,float Y,float Z)
{
  D3DXMATRIX World;
  D3DXMatrixIdentity(&World);
  D3DXMatrixTranslation(&World,X,Y,Z);
  _WrldMatrix = _ScaleMatrix*_RollMatrix*World;
  _ObjectToView = _WrldMatrix*gr_Camera->View;
} //EndDispositionModelProcedure
*/
//-----------------------------------------------------------------------------