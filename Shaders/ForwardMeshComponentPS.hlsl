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
    
    float3 toEye = normalize(viewPosition - input.f4ModelPos.xyz);
    float3 toLight = -reflect(toEye, normal);
    
    float VDotH = max(dot(normal, toEye), 0.f);
    
    float3 f0 = lerp(specular, diffuse, metallic);
    float3 ks = SchlickFresnelReflectRoughnessTerm(f0, max(0.0, dot(toLight, normal)), roughness);
    float3 kd = (1.0f - ks) * (1 - metallic);
    
    
    float3 color = CalculateIBL(
        kd, ks, diffuse, metallic, roughness,
        normal, toLight, VDotH,
        specularIBLTexture, diffuseIBLTexture, brdfLUTTexture, clampSampler
    ) * ambientOcculusion + emissive;
    
    
    // Direct Lighting Test ==============================================================
    float3 spotLightPos = float3(0.f, 0.f, 0.f);
    float3 toSpotLight = normalize(spotLightPos - input.f4ModelPos.xyz);
    float3 spotLightPower = 1.f;
    float3 spotHalfVector = (toEye + toSpotLight) / 2.f;
    
    float spotLDotH = max(0.f, dot(toSpotLight, spotHalfVector));
    float spotNDotH = max(0.f, dot(normal, spotHalfVector));
    float spotLDotN = max(0.f, dot(toSpotLight, normal));
    float spotVDotN = max(0.f, dot(toEye, normal));
    
    float3 directDiffuseBRDF = (kd * diffuse / 3.141592);
    float3 directSpecularBRDF = SchlickFresnelReflectRoughnessTerm(ks, spotLDotH, roughness)
    * SchlickGeometryModel(spotLDotN, spotVDotN, roughness)
    * NormalDistributionGGXTerm(spotNDotH, roughness)
    / max((4 * spotLDotN * spotVDotN), 1E-6);
    
    color += (directDiffuseBRDF + directSpecularBRDF) * spotLightPower * spotLDotN;
    // ===================================================================================
    
    Result.f4Color = float4(color, 1.f);   
    Result.uiID = IDValues;
    
    return Result;
}