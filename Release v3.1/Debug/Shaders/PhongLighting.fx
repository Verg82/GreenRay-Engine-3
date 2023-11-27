//-----------------------------------------------------------------------------
// File: HDRLighting.fx
//
// Desc: Effect file for High Dynamic Range Lighting sample. This file contains
//       version 2.0 pixel and vertex shaders in High-Level Shader Language.
//       These shaders are used to quickly calculate the average luminance
//       of the rendered scene, simulate the viewer's light adaptation level,
//       map the high dynamic range of colors to a range displayable on a PC
//       monitor, and perform post-process lighting effects. 
//
// The algorithms described in this sample are based very closely on the 
// lighting effects 
// implemented in Masaki Kawase's Rthdribl sample and the tone 
// mapping process described in the whitepaper 
// "Tone Reproduction for Digital Dreams Inc., 2006// Images"
//
// Real-Time High Dynamic Range Image-Based Lighting (Rthdribl)
// Masaki Kawase
// http://www.daionet.gr.jp/~masa/rthdribl/ 
//
// "Photographic Tone Reproduction for Digital Dreams Development"
// Erik Reinhard, Mike Stark, Peter Shirley and Jim Ferwerda
// http://www.cs.utah.edu/~reinhard/cdrom/ 
// 
// Copyright (c) Microsoft Corporation. All rights reserved.
//-----------------------------------------------------------------------------




//-----------------------------------------------------------------------------
// Global constants
//-----------------------------------------------------------------------------
static const int NUM_LIGHTS = 3;     // Scene lights 

//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------

// Transformation matrices
float4x4 g_mObjectToView;   // Object space to view space
float4x4 g_mProjection;     // View space to clip space

bool g_bEnableTexture;   // Toggle texture modulation for current pixel

// Light variables
float4 g_avLightPositionView[NUM_LIGHTS];   // Light positions in view space
float4 g_avLightIntensity[NUM_LIGHTS];      // Floating point light intensities

float  g_fPhongExponent;        // Exponents for the phong equation
float  g_fPhongCoefficient;     // Coefficient for the phong equation
float  g_fDiffuseCoefficient;   // Coefficient for diffuse equation
float4 g_vEmissive;             // Emissive intensity of the current light







//-----------------------------------------------------------------------------
// Texture samplers
//-----------------------------------------------------------------------------
sampler s0 : register(s0);
sampler s1 : register(s1);
sampler s2 : register(s2);
sampler s3 : register(s3);
sampler s4 : register(s4);
sampler s5 : register(s5);
sampler s6 : register(s6);
sampler s7 : register(s7);

//-----------------------------------------------------------------------------
// Vertex shaders
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Name: TransformScene     
// Type: Vertex shader                                      
// Desc: Transforms the incoming vertex from object to clip space, and passes
//       the vertex position and normal in view space on to the pixel shader
//-----------------------------------------------------------------------------
struct TransformSceneOutput
{
    float4 Position : POSITION;
    float2 Texture0 : TEXCOORD0;
    float3 Texture1 : TEXCOORD1;
    float3 Texture2 : TEXCOORD2;
};

TransformSceneOutput TransformScene
    (
    float3 vObjectPosition : POSITION, 
    float3 vObjectNormal : NORMAL,
    float2 vObjectTexture : TEXCOORD0
    )
{
    TransformSceneOutput Output;
    float4 vViewPosition;
    float3 vViewNormal;
  
    // tranform the position/normal into view space
    vViewPosition = mul(float4(vObjectPosition, 1.0f), g_mObjectToView);
    vViewNormal = normalize(mul(vObjectNormal, (float3x3)g_mObjectToView));

    // project view space to screen space
    Output.Position = mul(vViewPosition, g_mProjection);
    
    // Pass the texture coordinate without modification
    Output.Texture0 = vObjectTexture;

    // Pass view position into a texture iterator
    Output.Texture1 = vViewPosition.xyz;
    
    // Pass view surface normal into a texture iterator
    Output.Texture2 = vViewNormal;
    
    return Output;
}



//-----------------------------------------------------------------------------
// Pixel shaders
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Name: PointLight                                        
// Type: Pixel shader
// Desc: Per-pixel diffuse, specular, and emissive lighting
//-----------------------------------------------------------------------------
float4 PointLight
    (
    in float2 vTexture : TEXCOORD0,
    in float3 vViewPosition : TEXCOORD1,
    in float3 vNormal : TEXCOORD2
    ) : COLOR
{
    float3 vPointToCamera = normalize(-vViewPosition);

    // Start with ambient term
    float3 vIntensity = float3(0.28f, 0.25f, 0.2f);

    // Add emissive term to the total intensity
    vIntensity+= g_vEmissive; 
        
    for(int iLight = 0; iLight < NUM_LIGHTS; iLight++)
    {
        // Calculate illumination variables
        float3 vLightToPoint = normalize(vViewPosition - g_avLightPositionView[iLight]);
        float3 vReflection   = reflect(vLightToPoint, vNormal);
        float  fPhongValue   = saturate(dot(vReflection, vPointToCamera));

        // Calculate diffuse term
        float  fDiffuse      = g_fDiffuseCoefficient * saturate(dot(vNormal, -vLightToPoint));

        // Calculate specular term
        float  fSpecular     = g_fPhongCoefficient * pow(fPhongValue, g_fPhongExponent);
        
        // Scale according to distance from the light
        float fDistance = distance(g_avLightPositionView[iLight], vViewPosition);
        vIntensity += (fDiffuse + fSpecular) * g_avLightIntensity[iLight]/(fDistance*fDistance);
    }
    
    // Multiply by texture color
    if( g_bEnableTexture )
        vIntensity *= tex2D(s0, vTexture);

    return float4(vIntensity, 1.0f);
}





//-----------------------------------------------------------------------------
// Techniques
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Name: RenderScene
// Type: Technique                                     
// Desc: Performs specular lighting
//-----------------------------------------------------------------------------
technique RenderScene
{
    pass P0
    {  
        VertexShader = compile vs_2_0 TransformScene();
        PixelShader  = compile ps_2_0 PointLight();
    }
}


