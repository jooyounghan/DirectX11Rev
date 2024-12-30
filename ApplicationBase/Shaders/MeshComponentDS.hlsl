#include "MeshComponentHeader.hlsli"

cbuffer CameraViewConstantBuffer : register(b0)
{
    matrix ViewProjMatrix;
    matrix ViewProjInvMatrix;
    float3 ViewPosition;
    float Dummy;
};

cbuffer ModelConstantBuffer : register(b1)
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
    float HeightScale;
};

Texture2D HeightMap : register(t0);

SamplerState WrapSampler : register(s0);
SamplerState ClampSampler : register(s1);

[domain("tri")]
MeshComponentDomainOutput main(
	PatchTess input,
	float3 domain : SV_DomainLocation, const OutputPatch<MeshComponentHullOutput, NUM_CONTROL_POINTS> tri)
{
    MeshComponentDomainOutput Result;

    Result.f2TexCoord = tri[0].f2TexCoord * domain.x + tri[1].f2TexCoord * domain.y + tri[2].f2TexCoord * domain.z;
    Result.fLODLevel = tri[0].fLODLevel * domain.x + tri[1].fLODLevel * domain.y + tri[2].fLODLevel * domain.z;

    float3 f3WorldNormal = normalize(tri[0].f3WorldNormal * domain.x + tri[1].f3WorldNormal * domain.y + tri[2].f3WorldNormal * domain.z);
    Result.f3ModelTangent = normalize(tri[0].f3ModelTangent * domain.x + tri[1].f3ModelTangent * domain.y + tri[2].f3ModelTangent * domain.z);
	
    float4 f4WorldPos = tri[0].f4WorldPos * domain.x + tri[1].f4WorldPos * domain.y + tri[2].f4WorldPos * domain.z;

    if (IsHeightSet)
    {
        float Height =HeightMap.SampleLevel(WrapSampler, Result.f2TexCoord, Result.fLODLevel).x;
        Height = (2.f * Height - 1.f) * HeightScale;
        f4WorldPos += float4(Height * f3WorldNormal, 0.f);
    }

    Result.f3ModelNormal = normalize(mul(float4(f3WorldNormal, 0.f), ModelInvMatrix).xyz);

    Result.f4ModelPos = mul(f4WorldPos, ModelMatrix);
    Result.f4ProjPos = mul(Result.f4ModelPos, ViewProjMatrix);
    
    return Result;
}
