#include "MeshComponentHeader.hlsli"
#include "MaterialComponentHeader.hlsli"
#include "PBRHeader.hlsli"

cbuffer PickingID : register(b0)
{
    uint4 IDValues;
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

TextureCube SpecularTexture : register(t0);
TextureCube DiffuseTexture : register(t1);
Texture2D BRDFTexture : register(t2);

Texture2D MaterialTexture[7] : register(t3);

SamplerState WrapSampler : register(s0);

DefferedMeshComponentPixelOutput main(MeshComponentDomainOutput Input)
{
    DefferedMeshComponentPixelOutput Result;
    
    float3 BaseColor = IsDiffuseSet ? MaterialTexture[DIFFUSE_IDX].SampleLevel(WrapSampler, Input.f2TexCoord, Input.fLODLevel).rgb : float3(1.0, 1.0, 1.0);
    float AmbientOcculusion = IsAmbientOcculusionSet ? MaterialTexture[AO_IDX].SampleLevel(WrapSampler, Input.f2TexCoord, Input.fLODLevel).r : 1.0f;
    float Metallic = IsMetalicSet ? MaterialTexture[METALIC_IDX].SampleLevel(WrapSampler, Input.f2TexCoord, Input.fLODLevel).r : 0.0f;
    float Roughness = IsRoughnessSet ? MaterialTexture[ROUGHNESS_IDX].SampleLevel(WrapSampler, Input.f2TexCoord, Input.fLODLevel).r : 1.0f;
    float3 Emissive = IsEmissiveSet ? MaterialTexture[EMISSIVE_IDX].SampleLevel(WrapSampler, Input.f2TexCoord, Input.fLODLevel).rgb : float3(0.0, 0.0, 0.0);
    
    float3 Normal = Input.f3ModelNormal;
    if (IsNormalSet)
    {
        float3 Bitangent = normalize(cross(Input.f3ModelNormal, Input.f3ModelTangent));
        Normal = GetNormalFromMap(Input.f3ModelNormal, Bitangent, Input.f3ModelTangent, Input.fLODLevel, Input.f2TexCoord, MaterialTexture[NORMAL_IDX], WrapSampler);
    }
    
    Result.f4Position = Input.f4ModelPos;
    Result.f4BaseColor = float4(BaseColor, 1.f);
    Result.f4Normal = float4(Normal, 1.f);
    Result.AO_Metallic_Roughness = float4(AmbientOcculusion, Metallic, Roughness, 1.f);
    Result.Emissive = float4(Emissive, 1.f);
    Result.f4ID = IDValues;
    
    return Result;
}