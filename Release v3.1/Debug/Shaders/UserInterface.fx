/****************************************************************************/ 
// It's Unofficial Version Of The GreenRay Engine v.3.0.0.2
// UserInterface.fx
// This Effect Contains The User Interface Shaders.
// Date Creation: 29 August 2005
// Last Changing: 30 August 2007
// Author Rights By: Zie Technology And Lucky's Flash Studio Inc.,
// Licensed By: (C)DDD Digital Dreams Development Inc., 2007
/*****************************************************************************/   

float4x4 Project_Matrix: WORLDVIEWPROJECTION;
float4 UIBlend_Weight;
texture Input_Texture;

sampler TextureSampler = sampler_state
{
  Texture = <Input_Texture>;
  //MinFilter = LINEAR;  
  //MagFilter = LINEAR;
  //MipFilter = LINEAR;
};

//-----------------------------------------------------------------------------
// Name: VS_INPUT
// Desc: This Structure Contain Vertex Shader Input Registers
//-----------------------------------------------------------------------------
struct VS_INPUT
{
  float4 Position: POSITION;
  float4 Diffuse: COLOR;
  float3 Normal: NORMAL;
  float4 Tex0: TEXCOORD0;
};

//-----------------------------------------------------------------------------
// Name: VS_OUTPUT
// Desc: This Structure Contain Vertex Shader Output Registers
//-----------------------------------------------------------------------------
struct VS_OUTPUT
{
  float4 Position: POSITION;
  float4 Diffuse: COLOR;
  float4 Tex0: TEXCOORD0;
};

//-----------------------------------------------------------------------------
// Name: PS_INPUT
// Desc: This Structure Contain Pixel Shader Input Registers
//-----------------------------------------------------------------------------
struct PS_INPUT
{
  float4 Diffuse: COLOR;
  float4 Tex0: TEXCOORD0;
};

//-----------------------------------------------------------------------------
// Name: MiscVertexShader()
// Desc: Here Place Input Shader Registers Data
//-----------------------------------------------------------------------------
VS_OUTPUT MiscVertexShader(VS_INPUT In)
{
  VS_OUTPUT Out = (VS_OUTPUT)0; 
  float4 pos = In.Position;

  //Transform Position To Screen Space
  Out.Position = mul(pos,Project_Matrix);
  Out.Diffuse = In.Diffuse; 
  Out.Tex0 = In.Tex0;

  return Out;
}

//-----------------------------------------------------------------------------
// Name: DefaultPixelShader()
// Desc: 
//-----------------------------------------------------------------------------
float4 DefaultPixelShader(PS_INPUT In): COLOR
{
  float4 c = {.5,0,0,0};
  c = In.Diffuse;
  return c;
}

//-----------------------------------------------------------------------------
// Name: MenuPixelShader()
// Desc: 
//-----------------------------------------------------------------------------
float4 MenuPixelShader(PS_INPUT In): COLOR
{
  float4 _Texture = tex2D(TextureSampler,In.Tex0);  
  float4 c = {0.99, .99, .99, .63250};
    
  c.a = UIBlend_Weight.a;
    
  return c*_Texture;
}

//-----------------------------------------------------------------------------

technique UserInterfaceTech
{
  pass P0
  {   
    ALPHABLENDENABLE = TRUE;
    SRCBLEND         = SRCALPHA;
    DESTBLEND        = INVSRCALPHA;
    ZWriteEnable     = FALSE;
    ZFunc            = ALWAYS;
    VertexShader = compile vs_2_0 MiscVertexShader();
    PixelShader = compile ps_2_0 MenuPixelShader();
  }
}

//-----------------------------------------------------------------------------
