#include "MeshObjectHeader.hlsli"
#include "MaterialHeader.hlsli"
#include "PBRHeader.hlsli"

cbuffer CameraViewProj : register(b0)
{
    matrix ViewProjMatrix;
    matrix ViewProjInvMatrix;
    float3 ViewPosition;
    float Dummy;
};

cbuffer MaterialAssetData : register(b1)
{
    bool    IsAmbientOcculusionSet;
    bool    IsSpecularSet;
    bool    IsDiffuseSet;
    bool    IsRoughnessSet;
    bool    IsMetalicSet;
    bool    IsNormalSet;
    bool    IsHeightSet;
    bool    IsEmissiveSet;
    float3  F0;
    bool    Dummy1;
};

TextureCube SpecularTexture : register(t0);
TextureCube DiffuseTexture : register(t1);
Texture2D BRDFTexture : register(t2);

Texture2D MaterialTexture[6] : register(t3);

SamplerState WrapSampler : register(s0);
SamplerState ClampSampler : register(s1);

float4 main(MeshObjectVertexOutput Input) : SV_TARGET
{    
    float LODLevel = 2.f;
    
    float3 ToEye = normalize(ViewPosition - Input.f4ProjPos.xyz);
    
    float3 BaseColor = MaterialTexture[DIFFUSE_IDX].SampleLevel(WrapSampler, Input.f2TexCoord, LODLevel).rgb;
    
    float AmbientOcculusion = 1.f;
    float Metallic = 0.f;
    float Roughness = 1.f;

    if (IsAmbientOcculusionSet)
    {
        AmbientOcculusion = MaterialTexture[AO_IDX].SampleLevel(WrapSampler, Input.f2TexCoord, LODLevel).r;
    }
    if (IsMetalicSet)
    {
        Metallic = MaterialTexture[METALIC_IDX].SampleLevel(WrapSampler, Input.f2TexCoord, LODLevel).r;
    }
    if (IsRoughnessSet)
    {
        Roughness = MaterialTexture[ROUGHNESS_IDX].SampleLevel(WrapSampler, Input.f2TexCoord, LODLevel).r;
    }

    float3 Emissive = MaterialTexture[EMISSIVE_IDX].SampleLevel(WrapSampler, Input.f2TexCoord, LODLevel).rgb;
    
    return float4(CalculateIBL(0.02, BaseColor, AmbientOcculusion, Metallic, Roughness, Input.f3ModelNormal, ToEye, SpecularTexture, DiffuseTexture, BRDFTexture, WrapSampler) + Emissive, 1.f);
}