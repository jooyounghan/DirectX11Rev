#include "MeshComponentHeader.hlsli"

cbuffer CameraViewConstantBuffer : register(b0)
{
    matrix viewProjMatrix;
    matrix viewProjInvMatrix;
    float3 viewPosition;
    float dummy;
};

cbuffer ModelConstantBuffer : register(b1)
{
    matrix modelMatrix;
    matrix modelInvMatrix;
};

cbuffer MaterialAssetData : register(b2)
{
    bool isAmbientOcculusionSet;
    bool isSpecularSet;
    bool isDiffuseSet;
    bool isRoughnessSet;
    bool isMetalicSet;
    bool isNormalSet;
    bool isHeightSet;
    bool isEmissiveSet;
    float3 f0;
    float heightScale;
};

Texture2D heightMap : register(t0);

SamplerState wrapSampler : register(s0);

[domain("tri")]
MeshComponentDomainOutput main(
	PatchTess input,
	float3 domain : SV_DomainLocation, const OutputPatch<MeshComponentHullOutput, NUM_CONTROL_POINTS> tri)
{
    MeshComponentDomainOutput result;

    float4 f4WorldPos = tri[0].f4WorldPos * domain.x + tri[1].f4WorldPos * domain.y + tri[2].f4WorldPos * domain.z;
    result.f2TexCoord = tri[0].f2TexCoord * domain.x + tri[1].f2TexCoord * domain.y + tri[2].f2TexCoord * domain.z;
    float3 f3WorldNormal = normalize(tri[0].f3WorldNormal * domain.x + tri[1].f3WorldNormal * domain.y + tri[2].f3WorldNormal * domain.z);
    result.f3ModelNormal = normalize(mul(float4(f3WorldNormal, 0.f), modelInvMatrix).xyz);

    if (isHeightSet)
    {
        float height = heightMap.SampleLevel(wrapSampler, result.f2TexCoord, 0.f).x;
        height = (2.f * height - 1.f) * heightScale;
        f4WorldPos += float4(height * f3WorldNormal, 0.f);
    }
    
    result.f4ModelPos = mul(f4WorldPos, modelMatrix);
    result.f4ProjPos = mul(result.f4ModelPos, viewProjMatrix);
    
    return result;
}
