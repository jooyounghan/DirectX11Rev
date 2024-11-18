#include "MeshObjectHeader.hlsli"
#include "MaterialHeader.hlsli"
#include "PBRHeader.hlsli"

cbuffer PickingID : register(b0)
{
    float4 IDValues;
};

cbuffer CameraViewConstantBuffer : register(b1)
{
    matrix ViewProjMatrix;
    matrix ViewProjInvMatrix;
    float3 ViewPosition;
    float Dummy;
};

cbuffer MaterialAssetData : register(b2)
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

Texture2D MaterialTexture[7] : register(t3);

SamplerState WrapSampler : register(s0);
SamplerState ClampSampler : register(s1);

MeshObjectPixelOutput main(MeshObjectDomainOutput Input) : SV_TARGET
{        
    MeshObjectPixelOutput Result;
    
    float3 ToEye = normalize(ViewPosition - Input.f4ModelPos.xyz);
    
    float3 BaseColor = MaterialTexture[DIFFUSE_IDX].SampleLevel(WrapSampler, Input.f2TexCoord, Input.fLODLevel).rgb;
    float AmbientOcculusion = 1.f;
    float Metallic = 0.f;
    float Roughness = 1.f;
    float3 Normal = Input.f3ModelNormal;
    
    if (IsAmbientOcculusionSet)
    {
        AmbientOcculusion = MaterialTexture[AO_IDX].SampleLevel(WrapSampler, Input.f2TexCoord, Input.fLODLevel).r;
    }
    if (IsMetalicSet)
    {
        Metallic = MaterialTexture[METALIC_IDX].SampleLevel(WrapSampler, Input.f2TexCoord, Input.fLODLevel).r;
    }
    if (IsRoughnessSet)
    {
        Roughness = MaterialTexture[ROUGHNESS_IDX].SampleLevel(WrapSampler, Input.f2TexCoord, Input.fLODLevel).r;
    }
    if (IsNormalSet)
    {
        float3 Bitangent = normalize(cross(Input.f3ModelNormal, Input.f3ModelTangent));
        Normal = GetNormalFromMap(Input.f3ModelNormal, Bitangent, Input.f3ModelTangent, Input.fLODLevel, Input.f2TexCoord, MaterialTexture[NORMAL_IDX], WrapSampler);
    }
 
    float3 Emissive = MaterialTexture[EMISSIVE_IDX].SampleLevel(WrapSampler, Input.f2TexCoord, Input.fLODLevel).rgb;
    
    Result.f4Color = float4(CalculateIBL(F0, BaseColor, AmbientOcculusion, Metallic, Roughness, Normal, ToEye, SpecularTexture, DiffuseTexture, BRDFTexture, WrapSampler) + Emissive, 1.f);
    Result.f4ID = IDValues;
    return Result;
}