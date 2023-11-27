//////////////////////////////////////////////////////////////////////////////
// File: PointLight.fx
// Author: Verg
// Date Created: 30/08/2007
// Description: Contains Shaders To Render Dynamic Point Light Effect
//////////////////////////////////////////////////////////////////////////////

float4x4 WorldViewProjection;    // World*View*Projection matrix
float4x4 WorldView;              // World*View
float4x4 Proj;                   // Projection Matrix
float4   AmbientA;               // Ambient Light Color
float4   LightColor;             // Color Of The Light
float4   MatColor;               // Color Of The Material
float4   LightView;              // Position Of Light In View Space
float    FarClip;                // Z Of Far Clip Plane
texture  Texture;                // Texture For Scene Rendering

float4x4 World;         //World
float4x4 TexTransform;  //Texture transformation matrix
float3 EyePos;          //World position of the camera
//float Ambient_ = 0.0f;  //Amount of ambient light
float LightFalloff;     //Range of the light
texture LightTex;


float LIGHT_FALLOFF = 17.0f;

sampler textureSampler = sampler_state
{
    Texture = <Texture>;
    MinFilter = Linear;
    MagFilter = Linear;
    MipFilter = Linear;
};



//Must be clamped so the light texture shows up only once
sampler LightTexSampler = sampler_state
{
    Texture = <LightTex>;
    MinFilter = LINEAR;  
    MagFilter = LINEAR;
    MipFilter = LINEAR;
    AddressU  = Clamp;
    AddressV  = Clamp;
};

sampler ColorTexSampler = sampler_state
{
    Texture = <Texture>;
    MinFilter = LINEAR;  
    MagFilter = LINEAR;
    MipFilter = LINEAR;
    AddressU  = Wrap;
    AddressV  = Wrap;
};


//////////////////////////////////////////////////////////////////////////////
// Structures
//////////////////////////////////////////////////////////////////////////////

struct AmbientA2V
{ 
  float4 Position: POSITION;
  float2 TexCoord0: TEXCOORD0;
};

struct AmbientV2P
{
  float4 Position: POSITION;
  float2 TexCoord0: TEXCOORD0;
};

struct PointLightA2V
{ 
  float4 Position: POSITION;
  float3 Normal: NORMAL;
  float2 TexCoord0: TEXCOORD0;
};

struct PointLightV2P
{
  float4 Position: POSITION;
  float3 LightDir: TEXCOORD0;
  float3 ViewNormal: TEXCOORD1;
  float2 TexCoord0: TEXCOORD2;
  float4 Diffuse: TEXCOORD3;
};



struct MainA2V{
    float4 Position : POSITION;
    float2 TexCoord0 : TEXCOORD0;
};

struct MainV2P{
    float4 Position  : POSITION;
    float2 TexCoord0 : TEXCOORD0;
    float4 TexCoord1 : TEXCOORD1;
    float3 LightVec  : TEXCOORD2;
};


//////////////////////////////////////////////////////////////////////////////
// Vertex Shaders
//////////////////////////////////////////////////////////////////////////////

void AmbientVS(in AmbientA2V IN,out AmbientV2P OUT)
{
  //Transform The Position From Object Space To Homogeneous Projection Space
  OUT.Position = mul(IN.Position,WorldViewProjection);

  //Copy The Texture Coordinate Through
  OUT.TexCoord0 = IN.TexCoord0;
}

void PointLightVS(in PointLightA2V IN,out PointLightV2P OUT)
{
  //Transform The Position From View Space To Homogeneous Projection Space
  OUT.Position = mul(IN.Position,WorldViewProjection);

  //Compute View Space Position
  OUT.LightDir = LightView-mul(IN.Position,WorldView);

  //Compute World Space Normal
  OUT.ViewNormal = normalize(mul(IN.Normal,(float3x3)WorldView));

  //Modulate Material With Light To Obtain Diffuse
  OUT.Diffuse = MatColor*LightColor;

  //Copy The Texture Coordinate Through
  OUT.TexCoord0 = IN.TexCoord0;
}



void MainVS(in MainA2V IN, out MainV2P OUT)
{
    //Copy color texture coordinates through
    OUT.TexCoord0 = IN.TexCoord0;
    
    //First two are the projected texture coordinates
    OUT.TexCoord1 = mul(IN.Position, TexTransform);
    
    //Position to light vector
    OUT.LightVec = mul(IN.Position.xyz, (float3x3)World) - EyePos;
    
    //Transform model-space vertex position to screen space:
    OUT.Position = mul(IN.Position, WorldViewProjection);
}

//////////////////////////////////////////////////////////////////////////////
// Pixel Shaders
//////////////////////////////////////////////////////////////////////////////

float4 AmbientPS(in AmbientV2P IN): COLOR0
{
  //Lookup Mesh Texture And Modulate It With Material And Ambient Amount
  return AmbientA*tex2D(textureSampler,IN.TexCoord0);
}

float4 PointLightPS(in PointLightV2P IN): COLOR0
{
  //Pixel To Light Vector
  float LenSq = dot(IN.LightDir,IN.LightDir);
  IN.LightDir = normalize(IN.LightDir);
    
  float Attn = min((LIGHT_FALLOFF*LIGHT_FALLOFF)/LenSq,1.0f);

  //Compute Lighting Amount
  float4 I = saturate(dot(normalize(IN.ViewNormal),IN.LightDir))*IN.Diffuse*Attn;

  //Lookup Mesh Texture And Modulate It With Diffuse
  return float4(tex2D(textureSampler,IN.TexCoord0).xyz,1.0f)*I;
}



float4 MainPS(in MainV2P IN) : COLOR 
{
    //Read in values for light texture and color texture
    float4 Color    = tex2D(ColorTexSampler, IN.TexCoord0);
    float4 Light    = tex2Dproj(LightTexSampler, IN.TexCoord1);
    
    //Squared distance from pixel to light
    float LenSq = dot( IN.LightVec, IN.LightVec );
        
    //Find the attenuation
    float Attn = min(( LightFalloff * LightFalloff ) / LenSq, 1.0f);
    
    //Compute lighting amount
    float4 I = (Light * Attn); //+ Ambient_;
    
    //Color of texture * lighting amount
    return Color * I;
}


//////////////////////////////////////////////////////////////////////////////
// Techniques
//////////////////////////////////////////////////////////////////////////////

technique Ambient
{
  pass P0
  {
    VertexShader = compile vs_1_1 AmbientVS();
    PixelShader = compile ps_1_1 AmbientPS();
    StencilEnable = false;
    ZFunc = LessEqual;
  }
}

technique PointLight
{
  pass P0
  {
    VertexShader = compile vs_1_1 PointLightVS();
    PixelShader = compile ps_2_0 PointLightPS();
    ZEnable = true;
    ZFunc = LessEqual;
    StencilEnable = true;
    AlphaBlendEnable = true;
    BlendOp = Add;
    SrcBlend = One;
    DestBlend = One;
    StencilRef = 1;
    StencilFunc = Greater;
    StencilPass = Keep;
  }
    
  pass P1
  {
    VertexShader = compile vs_1_1 MainVS();
    PixelShader = compile ps_2_0 MainPS();
  }


}


//technique ProjectiveTexture
//{
//    pass P0
//    {
//        VertexShader = compile vs_1_1 MainVS();
//        PixelShader = compile ps_2_0 MainPS();
//    }
//}
