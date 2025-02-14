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
    bool    isAmbientOcculusionSet;
    bool    isSpecularSet;
    bool    isDiffuseSet;
    bool    isRoughnessSet;
    bool    isMetalicSet;
    bool    isNormalSet;
    bool    isHeightSet;
    bool    isEmissiveSet;
    float3  f0;
    bool    dummy1;
};

TextureCube specularIBLTexture : register(t0);
TextureCube diffuseIBLTexture : register(t1);
Texture2D brdfLUTTexture : register(t2);

Texture2D materialTexture[7] : register(t3);

SamplerState wrapSampler : register(s0);
SamplerState clampSampler : register(s1);

MeshComponentPixelOutput main(MeshComponentDomainOutput input) : SV_TARGET
{        
    MeshComponentPixelOutput Result;
    
    float ambientOcculusion = isAmbientOcculusionSet ? materialTexture[AO_IDX].SampleLevel(wrapSampler, input.f2TexCoord, input.fLODLevel).r : 1.0f;
    float3 specular = isSpecularSet ? materialTexture[SPECULAR_IDX].SampleLevel(wrapSampler, input.f2TexCoord, input.fLODLevel).rgb : f0;
    float3 diffuse = isDiffuseSet ? materialTexture[DIFFUSE_IDX].SampleLevel(wrapSampler, input.f2TexCoord, input.fLODLevel).rgb : float3(1.0, 1.0, 1.0);
    float metallic = isMetalicSet ? materialTexture[METALIC_IDX].SampleLevel(wrapSampler, input.f2TexCoord, input.fLODLevel).r : 0.0f;
    float roughness = isRoughnessSet ? materialTexture[ROUGHNESS_IDX].SampleLevel(wrapSampler, input.f2TexCoord, input.fLODLevel).r : 1.0f;
    float3 emissive = isEmissiveSet ? materialTexture[EMISSIVE_IDX].SampleLevel(wrapSampler, input.f2TexCoord, input.fLODLevel).rgb : float3(0.0, 0.0, 0.0);
    float3 normal = input.f3ModelNormal;
    if (isNormalSet)
    {
        float3 bitangent = normalize(cross(input.f3ModelNormal, input.f3ModelTangent));
        normal = GetNormalFromMap(input.f3ModelNormal, bitangent, input.f3ModelTangent, input.fLODLevel, input.f2TexCoord, materialTexture[NORMAL_IDX], wrapSampler);
    }
    
    float3 toEye = normalize(viewPosition - input.f4ModelPos.xyz);
    float3 fromLight = -reflect(toEye, normal);
    
    float3 color = CalculateIBL(
        specular, diffuse, ambientOcculusion, metallic, roughness,
        normal, fromLight, toEye,
        specularIBLTexture, diffuseIBLTexture, brdfLUTTexture, clampSampler
    ) + emissive;
    
    
    Result.f4Color = float4(color, 1.f);   
    Result.uiID = IDValues;
    
    return Result;
}