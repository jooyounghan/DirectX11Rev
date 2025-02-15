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

    if (isHeightSet)
    {
        float height = heightMap.SampleLevel(wrapSampler, result.f2TexCoord, 0.f).x;
        height = (2.f * height - 1.f) * heightScale;
        f4WorldPos += float4(height * f3WorldNormal, 0.f);
    }
    
    result.f3ModelNormal = normalize(mul(float4(f3WorldNormal, 0.f), modelInvMatrix).xyz);
    
    float4 edge1 = tri[1].f4WorldPos - tri[0].f4WorldPos;
    float4 edge2 = tri[2].f4WorldPos - tri[0].f4WorldPos;

    float2 deltaUV1 = tri[1].f2TexCoord - tri[0].f2TexCoord;
    float2 deltaUV2 = tri[2].f2TexCoord - tri[0].f2TexCoord;
    
    float det = deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y;
    float f = (abs(det) < 1e-6f) ? 1.0f : 1.0f / det;

    result.f3ModelTangent = (edge1 * deltaUV2.y - edge2 * deltaUV1.y).xyz * f;
    
    float dotNT = dot(result.f3ModelNormal, result.f3ModelTangent);
    result.f3ModelTangent = normalize(result.f3ModelTangent - dotNT * result.f3ModelNormal);
    
    result.f4ModelPos = mul(f4WorldPos, modelMatrix);
    result.f4ProjPos = mul(result.f4ModelPos, viewProjMatrix);
    
    return result;
}
