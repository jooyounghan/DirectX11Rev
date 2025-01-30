#include "MeshComponentHeader.hlsli"
#include "MaterialComponentHeader.hlsli"
#include "PBRHeader.hlsli"

cbuffer ComponentBuffer : register(b0)
{
    uint IDValues;
    float3 DebugColor;
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

MeshComponentPixelOutput main(MeshComponentDomainOutput Input) : SV_TARGET
{        
    MeshComponentPixelOutput Result;
    
    float AmbientOcculusion = IsAmbientOcculusionSet ? MaterialTexture[AO_IDX].SampleLevel(WrapSampler, Input.f2TexCoord, Input.fLODLevel).r : 1.0f;
    float3 Specular = IsSpecularSet ? MaterialTexture[SPECULAR_IDX].SampleLevel(WrapSampler, Input.f2TexCoord, Input.fLODLevel).rgb : float3(0.0, 0.0, 0.0);
    float3 Diffuse = IsDiffuseSet ? MaterialTexture[DIFFUSE_IDX].SampleLevel(WrapSampler, Input.f2TexCoord, Input.fLODLevel).rgb : float3(1.0, 1.0, 1.0);
    float Metallic = IsMetalicSet ? MaterialTexture[METALIC_IDX].SampleLevel(WrapSampler, Input.f2TexCoord, Input.fLODLevel).r : 0.0f;
    float Roughness = IsRoughnessSet ? MaterialTexture[ROUGHNESS_IDX].SampleLevel(WrapSampler, Input.f2TexCoord, Input.fLODLevel).r : 1.0f;
    float3 Emissive = IsEmissiveSet ? MaterialTexture[EMISSIVE_IDX].SampleLevel(WrapSampler, Input.f2TexCoord, Input.fLODLevel).rgb : float3(0.0, 0.0, 0.0);
    float3 Normal = Input.f3ModelNormal;
    if (IsNormalSet)
    {
        float3 Bitangent = normalize(cross(Input.f3ModelNormal, Input.f3ModelTangent));
        Normal = GetNormalFromMap(Input.f3ModelNormal, Bitangent, Input.f3ModelTangent, Input.fLODLevel, Input.f2TexCoord, MaterialTexture[NORMAL_IDX], WrapSampler);
    }
    
    float3 ToEye = normalize(ViewPosition - Input.f4ModelPos.xyz);
    float3 ReflectDir = reflect(-ToEye, Normal);

    float3 IBLDiffuse = DiffuseTexture.Sample(WrapSampler, Normal).rgb;
    float3 IBLSpecular = SpecularTexture.Sample(WrapSampler, ReflectDir).rgb;
    
    float3 DiffuseIBL = IBLDiffuse * Diffuse;
    float3 SpecularIBL = IBLSpecular * Specular;
    
    Result.f4Color = float4(0.f, 0.f, 0.f, 0.f);
    Result.f4Color += float4(CalculateIBL(F0, Diffuse, AmbientOcculusion, Metallic, Roughness, Normal, ToEye, SpecularTexture, DiffuseTexture, BRDFTexture, WrapSampler) + Emissive, 1.f) / 2.f;
    Result.f4Color += float4((DiffuseIBL + SpecularIBL), 1.f) / 2.f;    
    Result.uiID = IDValues;
    
    return Result;
}