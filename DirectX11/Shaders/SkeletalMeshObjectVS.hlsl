#include "MeshObjectHeader.hlsli"
#include "PBRHeader.hlsli"

cbuffer CameraViewProj : register(b0)
{
    matrix ViewProjMatrix;
    matrix ViewProjInvMatrix;
    float3 ViewPosition;
    float Dummy;
};

cbuffer ModelMatrix : register(b1)
{
    matrix ModelMatrix;
    matrix ModelInvMatrix;
};

cbuffer MaterialAssetData : register(b2)
{
    bool IsAmbientOcculusionSet;
    bool IsSpecularSet;
    bool IsDiffuseSet;
    bool IsRoughnessSet;
    bool IsMetalicSet;
    bool IsNormalSet;
    bool IsHeightSet;
    bool IsEmissiveSet;
    float3 F0;
    bool Dummy1;
};

Texture2D NormalTexture : register(t0);
Texture2D HeightTexture : register(t1);

SamplerState WrapSampler : register(s0);
SamplerState ClampSampler : register(s1);

MeshObjectVertexOutput main(SkeletalVertexInput Input)
{
    MeshObjectVertexOutput Result;

    Result.f3ModelNormal = normalize(mul(float4(Input.f3WorldNormal, 0.f), ModelInvMatrix).xyz);
    float3 f3ModelTangent = normalize(mul(float4(Input.f3WorldTangent, 0.f), ModelMatrix).xyz);
    float3 f3ModelBitangent = normalize(mul(float4(Input.f3WorldBitangent, 0.f), ModelMatrix).xyz);
    
    if (IsNormalSet)
    {
        Result.f3ModelNormal = GetNormalFromMap(Result.f3ModelNormal, f3ModelBitangent, f3ModelTangent, 2.0, Input.f2TexCoord, NormalTexture, WrapSampler);
    }
    
    Result.f4ProjPos = mul(float4(Input.f3WorldPos, 1.f), ModelMatrix);
    Result.f4ProjPos = mul(Result.f4ProjPos, ViewProjMatrix);
    Result.f2TexCoord = Input.f2TexCoord;
    
    return Result;
}