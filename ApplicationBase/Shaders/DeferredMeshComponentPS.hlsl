#include "MeshComponentHeader.hlsli"
#include "MaterialComponentHeader.hlsli"
#include "PBRHeader.hlsli"

cbuffer ComponentBuffer : register(b0)
{
    uint IDValues;
    float3 debugColor;
};

cbuffer CameraViewConstantBuffer : register(b1)
{
    matrix viewProjMatrix;
    matrix viewProjInvMatrix;
    float3 viewPosition;
    float dummy;
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
    bool dummy1;
};

Texture2D materialTexture[7] : register(t0);

SamplerState wrapSampler : register(s0);

DeferredMeshComponentPixelOutput main(MeshComponentDomainOutput input)
{
    DeferredMeshComponentPixelOutput result;
    
    float ambientOcculusion = isAmbientOcculusionSet ? materialTexture[AO_IDX].Sample(wrapSampler, input.f2TexCoord).r : 1.0f;
    float3 specular = isSpecularSet ? materialTexture[SPECULAR_IDX].Sample(wrapSampler, input.f2TexCoord).rgb : f0;
    float3 diffuse = isDiffuseSet ? materialTexture[DIFFUSE_IDX].Sample(wrapSampler, input.f2TexCoord).rgb : float3(1.0, 1.0, 1.0);
    float metallic = isMetalicSet ? materialTexture[METALIC_IDX].Sample(wrapSampler, input.f2TexCoord).r : 0.0f;
    float roughness = isRoughnessSet ? materialTexture[ROUGHNESS_IDX].Sample(wrapSampler, input.f2TexCoord).r : 1.0f;
    float3 emissive = isEmissiveSet ? materialTexture[EMISSIVE_IDX].Sample(wrapSampler, input.f2TexCoord).rgb : float3(0.0, 0.0, 0.0);
    float3 normal = input.f3ModelNormal;
    
    float3 tangent;
    float3 bitangent;
    GetTB(input.f4ModelPos.xyz, input.f2TexCoord, input.f3ModelNormal, tangent, bitangent);
        
    if (isNormalSet)
    {
        normal = GetNormalFromMap(input.f3ModelNormal, bitangent, tangent, input.f2TexCoord, materialTexture[NORMAL_IDX], wrapSampler);
    }
    
    result.f4Position = input.f4ModelPos;    
    result.f4Specular = float4(specular, 1.f);
    result.f4Diffuse = float4(diffuse, 1.f);
    result.AO_Metallic_Roughness = float4(ambientOcculusion, metallic, roughness, 1.f);
    result.f4Normal = float4(normal, 1.f);
    result.Emissive = float4(emissive, 1.f);
    
    result.uiID = IDValues;
    
    return result;
}