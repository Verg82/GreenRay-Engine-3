/*******************************************************************************
// It's Unofficial Version Of The GreenRay Engine v.3.0.0.1
// GRShaders.cpp
// It's The GreenRay Shader Engine Source File.
// Date Creation: 29 August 2005
// Last Changing: 17 August 2006
// Author Rights By: Zie Technology, Zombie Soft And Lucky's Flash Studio Inc.,
// Licensed By: (C)DDD Digital Dreams Development And (C)3D Studios Inc., 2006
*******************************************************************************/
#include <GRShaders.h>
#include <GRMethods.h>

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction For CGRShaders Class
///////////////////////////////////////////////////////////////////////////////
CGRShaders::CGRShaders()
{

} //EndConstructionDirectives

CGRShaders::~CGRShaders()
{ 
} //EndDestructionDirectives

/*
//------------------------------------------------------------------------------
// LoadShaderFile
// Loads vertex shaders
//------------------------------------------------------------------------------
HRESULT LoadShaderFile(LPDIRECT3DDEVICE9 pd3dDevice,LPWSTR FileName,IDirect3DVertexShader9** shader,
IDirect3DVertexDeclaration9** declaration,D3DVERTEXELEMENT9 *decl)
{
    LPD3DXBUFFER pCode;
    HRESULT hr;
    LPD3DXBUFFER ErrorMessages;

    //--------------------//
    // Create vertex decl //
    //--------------------//
    if( FAILED( hr = pd3dDevice->CreateVertexDeclaration( decl, declaration ) ) )
    {
        return hr;
    }


    int n = strlen((char*)FileName);

    if( stricmp((char*)&FileName[n-4],"hlsl") == 0)
    {

        //------------------------------------------//
        // Compile HLSL vertex shader from the file //
        //------------------------------------------//
        if( FAILED( hr = D3DXCompileShaderFromFile(FileName, 0,0,"main", "vs_1_1",0,&pCode, & ErrorMessages,NULL ) ) )
        {
            //if( FAILED( hr = D3DXCompileShaderFromFile( filename, 0,0,"main", "vs_1_1",0,&pCode, & ErrorMessages,NULL ) ) )
            {
                //--------------------//
                // Inform user of err //
                //--------------------//
				ShowMessage("unShaders.cpp: Could Not Open A Vertex Shader File..");
               
    
                //--------------------------------------------------------//
                // Since the shader wont be created, lets prevent a leak  //
                //--------------------------------------------------------//
                ReleaseClass(*declaration);
                return hr;
            }
        }
    }
    else
    {
        //-------------------------------------------//
        // Assemble the vertex shader from the file  //
        //-------------------------------------------//
        if( FAILED( hr = D3DXAssembleShaderFromFile( FileName, 0,0,0, &pCode, & ErrorMessages ) ) )
        {
            //--------------------//
            // Inform user of err //
            //--------------------//
            ShowMessage("unShaders.cpp: Could Not Open A Vertex Shader File..");
        
    
            //-------------------------------------------------------//
            // Since the shader wont be created, lets prevent a leak //
            //-------------------------------------------------------//
            ReleaseClass(*declaration);
            return hr;
        }
    }
    //--------------------------//
    // Create the vertex shader //
    //--------------------------//
    hr = pd3dDevice->CreateVertexShader((DWORD*)pCode->GetBufferPointer(), shader);

    pCode->Release();
 
    return hr;
}

//------------------------------------------------------------------------------
// LoadShaderFile
// Loads pixel shaders
//------------------------------------------------------------------------------
HRESULT LoadShaderFile(LPDIRECT3DDEVICE9 pd3dDevice,LPWSTR FileName,IDirect3DPixelShader9** shader)
{
  TCHAR strShaderPath[512];
  LPD3DXBUFFER pCode;
  HRESULT hr;
  LPD3DXBUFFER ErrorMessages;

    ////////////////////////////
    // Construct shader name  //
    // /////////////////////////
    strncpy((char*)strShaderPath,(char*)FileName,512);
    int n = strlen((char*)FileName);

    if (stricmp((char*)&FileName[n-4],"hlsl") == 0)
    {

        //-----------------------------------------//
        // Compile HLSL pixel shader from the file //
        //-----------------------------------------//
        if( FAILED( hr = D3DXCompileShaderFromFile(FileName, 0,0,"main", "ps_2_0",0,&pCode, & ErrorMessages,NULL ) ) )
        {
//            if( FAILED( hr = D3DXCompileShaderFromFile( filename, 0,0,"main", "ps_1_4",0,&pCode, & ErrorMessages,NULL ) ) )
            {
                //--------------------//
                // Inform user of err //
                //--------------------//
				ShowMessage("unShaders.cpp: Could Not Open An HLSL Pixel Shader File..");
                return hr;
            }
        }
    }
    else
    {
        //-----------------------------------------//
        // Assemble the pixel shader from the file //
        //-----------------------------------------//
        if( FAILED( hr = D3DXAssembleShaderFromFile( strShaderPath, 0,0,D3DXSHADER_DEBUG, &pCode, &ErrorMessages ) ) )
        {
            //--------------------//
            // Inform user of err //
            //--------------------//
            ShowMessage("unShaders.cpp: Could Not Open An HLSL Pixel Shader File..");
            return hr;
        }
    }

    //-------------------------//
    // Create the pixel shader //
    //-------------------------//
    hr = pd3dDevice->CreatePixelShader( (DWORD*)pCode->GetBufferPointer(),shader);
    pCode->Release();

    return hr;
}
*/

//-----------------------------------------------------------------------------
// Name: CreateVertDeclaration()
// Desc: Universal Function For Create And Prepare Vertex Declarations
//-----------------------------------------------------------------------------
HRESULT CreateVertDeclaration(LPDIRECT3DDEVICE9 pDevice,IDirect3DVertexDeclaration9** Declaration,D3DVERTEXELEMENT9* Decl)
{
  HRESULT _Result;

  //Create Vertex Declaration
  if (Declaration != NULL)
  if (FAILED(_Result = pDevice->CreateVertexDeclaration(Decl,Declaration)))
  {
	ShowMessage("unShaders.cpp: Can't Create Vertex Declaration..");
    return _Result;
  }

  return _Result;  
} //EndCreateVertDeclarationFunction

//-----------------------------------------------------------------------------
// Name: LoadShaderFile()
// Desc: Loads Pixel Shaders
//-----------------------------------------------------------------------------
HRESULT LoadShaderFile(LPDIRECT3DDEVICE9 pDevice,LPCSTR FileName,LPD3DXEFFECT* Effect)
{
  LPD3DXBUFFER ErrorMessages;
  LPD3DXEFFECT TempEffect;
  HRESULT _Result;
  
  //Check On PixelShader 2.0 Present Parameters
  D3DCAPS9 _Caps;
  pDevice->GetDeviceCaps(&_Caps);
  if (FAILED(_Caps.PixelShaderVersion < D3DPS_VERSION(2,0)))
  {
    ShowMessage("unShaders.cpp: Your Video Card Not Support Pixel Shader Version 2.0..");
  }
  else 
  {
    if(FAILED(_Result = D3DXCreateEffectFromFile(pDevice,FileName,NULL,NULL,0,NULL,&TempEffect,&ErrorMessages)))
    {
      ShowMessage("unShaders.cpp: Could Not Create An HLSL Pixel Shader File..");      
      return _Result;
    }
  }

  *Effect = TempEffect;
  return _Result;  
} //EndLoadShaderFileFunction

//-----------------------------------------------------------------------------