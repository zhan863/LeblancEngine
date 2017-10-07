//======================================================================
//
//	Tone Mapping Sample
//  by MJP
//  http://mynameismjp.wordpress.com/
//
//  All code and content licensed under Microsoft Public License (Ms-PL)
//
//======================================================================

//======================================================================================
// Constant buffers
//======================================================================================
cbuffer VSConstants : register(cb0)
{
    float4x4 World : packoffset(c0);
    float4x4 WorldViewProjection : packoffset(c4);
}

cbuffer PSConstants : register(cb0)
{
    float3 DiffuseAlbedo : packoffset(c0);
    float3 SpecularAlbedo : packoffset(c1);
    float3 EnvMapBias : packoffset(c2);
    float3 CameraPosWS : packoffset(c3);
	float3 MainLightDirection : packoffset(c4);
}

//======================================================================================
// Samplers
//======================================================================================

TextureCube	EnvironmentMap : register(t0);
SamplerState AnisoSampler : register(s0);

//======================================================================================
// Input/Output structs
//======================================================================================

struct VSInput
{
    float4 PositionOS : POSITION;
    float3 NormalOS : NORMAL;    
};

struct VSOutput
{
    float4 PositionCS : SV_Position;
    float3 PositionWS : POSITIONWS;
    float3 NormalWS : NORMALWS;
};

//======================================================================================
// Vertex Shader
//======================================================================================
VSOutput VS(in VSInput input)
{
    VSOutput output;

    // Calc the world-space position
    output.PositionWS = mul(input.PositionOS, World).xyz;

    // Calc the clip-space position
    output.PositionCS = mul(input.PositionOS, WorldViewProjection);

    // Rotate the normal into world-space
    output.NormalWS = mul(input.NormalOS, World);

    return output;
}

//======================================================================================
// Pixel Shader
//======================================================================================
float4 PS(in VSOutput input) : SV_Target
{        
    // For diffuse lighting, we'll sample the 9th mip level (4x4) of the env. map using just the normal
    float3 normalWS = normalize(input.NormalWS);
    float3 diffuseLight = EnvironmentMap.SampleLevel(AnisoSampler, normalWS, 9.0f).rgb * EnvMapBias;
    float3 diffuseColor = diffuseLight * DiffuseAlbedo;

    // For specular lighting, we'll sample the env. map using the reflection vector
    float3 cameraToPixel = normalize(input.PositionWS - CameraPosWS);
    float3 reflectDir = reflect(cameraToPixel, normalWS);
    float3 specularLight = EnvironmentMap.Sample(AnisoSampler, reflectDir).rgb * EnvMapBias;
    float3 specularColor = specularLight * SpecularAlbedo;

    // Final color is diffuse + specular
    return float4(max(diffuseColor + specularColor, 0.0001f), 1.0f);    
}
