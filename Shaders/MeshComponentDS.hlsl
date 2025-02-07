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

[domain("tri")]
MeshComponentDomainOutput main(
	PatchTess input,
	float3 domain : SV_DomainLocation, const OutputPatch<MeshComponentHullOutput, NUM_CONTROL_POINTS> tri)
{
    MeshComponentDomainOutput Result;

    float4 f4WorldPos = tri[0].f4WorldPos * domain.x + tri[1].f4WorldPos * domain.y + tri[2].f4WorldPos * domain.z;
    Result.f2TexCoord = tri[0].f2TexCoord * domain.x + tri[1].f2TexCoord * domain.y + tri[2].f2TexCoord * domain.z;
    float3 f3WorldNormal = normalize(tri[0].f3WorldNormal * domain.x + tri[1].f3WorldNormal * domain.y + tri[2].f3WorldNormal * domain.z);
    Result.fLODLevel = tri[0].fLODLevel * domain.x + tri[1].fLODLevel * domain.y + tri[2].fLODLevel * domain.z;

    if (IsHeightSet)
    {
        float Height = HeightMap.SampleLevel(WrapSampler, Result.f2TexCoord, Result.fLODLevel).x;
        Height = (2.f * Height - 1.f) * HeightScale;
        f4WorldPos += float4(Height * f3WorldNormal, 0.f);
    }
    
    Result.f3ModelNormal = normalize(mul(float4(f3WorldNormal, 0.f), ModelInvMatrix).xyz);
    
    float4 edge1 = tri[1].f4WorldPos - tri[0].f4WorldPos;
    float4 edge2 = tri[2].f4WorldPos - tri[0].f4WorldPos;

    float2 deltaUV1 = tri[1].f2TexCoord - tri[0].f2TexCoord;
    float2 deltaUV2 = tri[2].f2TexCoord - tri[0].f2TexCoord;
    
    float det = deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y;
    float f = (abs(det) < 1e-6f) ? 1.0f : 1.0f / det;

    Result.f3ModelTangent = (edge1 * deltaUV2.y - edge2 * deltaUV1.y).xyz * f;
    
    float dotNT = dot(Result.f3ModelNormal, Result.f3ModelTangent);
    Result.f3ModelTangent = normalize(Result.f3ModelTangent - dotNT * Result.f3ModelNormal);

    Result.f4ModelPos = mul(f4WorldPos, ModelMatrix);
    Result.f4ProjPos = mul(Result.f4ModelPos, ViewProjMatrix);
    
    return Result;
}
