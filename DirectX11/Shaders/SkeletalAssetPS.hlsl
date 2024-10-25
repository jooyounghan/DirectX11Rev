#include "SkeletalAssetHeader.hlsli"
#include "MaterialHeader.hlsli"
#include "PBRHeader.hlsli"

cbuffer CameraViewProj : register(b0)
{
    matrix ViewProjMatrix;
    matrix ViewProjInvMatrix;
    float3 ViewPosition;
    float Dummy;
};

TextureCube SpecularTexture : register(t0);
TextureCube DiffuseTexture : register(t1);
Texture2D BRDFTexture : register(t2);

Texture2D MaterialTexture[8] : register(t3);

SamplerState WrapSampler : register(s0);
SamplerState ClampSampler : register(s1);

float4 main(BasicVertexOutput Input) : SV_TARGET
{    
    float LODLevel = 2.f;
    
    float3 ToEye = normalize(ViewPosition - Input.f4ProjPos.xyz);
    float3 Normal = GetNormalFromMap(
        Input.f3ModelNormal, Input.f3ModelBitangent, Input.f3ModelTangent, 
        LODLevel, Input.f2TexCoord, MaterialTexture[NORMAL_IDX], WrapSampler
    );
    
    float3 BaseColor = MaterialTexture[DIFFUSE_IDX].SampleLevel(WrapSampler, Input.f2TexCoord, LODLevel).rgb;
    float AmbientOcculusion = MaterialTexture[AO_IDX].SampleLevel(WrapSampler, Input.f2TexCoord, LODLevel).r;
    float Metallic = MaterialTexture[METALIC_IDX].SampleLevel(WrapSampler, Input.f2TexCoord, LODLevel).r;
    float Roughness = MaterialTexture[ROUGHNESS_IDX].SampleLevel(WrapSampler, Input.f2TexCoord, LODLevel).r;
    float3 Emissive = MaterialTexture[EMISSIVE_IDX].SampleLevel(WrapSampler, Input.f2TexCoord, LODLevel).rgb;
    
    return float4(CalculateIBL(0.02, BaseColor, AmbientOcculusion, Metallic, Roughness, Normal, ToEye, SpecularTexture, DiffuseTexture, BRDFTexture, WrapSampler), 1.f);
}