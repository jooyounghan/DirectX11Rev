#include "GBufferResolverHeader.hlsli"
#include "PBRHeader.hlsli"

cbuffer CameraViewConstantBuffer : register(b0)
{
    matrix ViewProjMatrix;
    matrix ViewProjInvMatrix;
    float3 ViewPosition;
    float Dummy;
};

TextureCube SpecularTexture : register(t0);
TextureCube DiffuseTexture : register(t1);
Texture2D BRDFTexture : register(t2);

Texture2D PositionGBuffer : register(t3);
Texture2D SpecularGBuffer : register(t4);
Texture2D DiffuseGBuffer : register(t5);
Texture2D AO_Metallic_Roughtness_GBuffer : register(t6);
Texture2D NormalGBuffer : register(t7);
Texture2D EmissiveGBuffer : register(t8);
Texture2D FresnelReflectanceGBuffer : register(t9);

SamplerState WrapSampler : register(s0);

float4 main(GBufferResolveVertexOutput Input) : SV_TARGET
{
    float3 Position = PositionGBuffer.Sample(WrapSampler, Input.f2TexCoord).xyz;
    float3 Specular = SpecularGBuffer.Sample(WrapSampler, Input.f2TexCoord).xyz;
    float3 Diffuse = DiffuseGBuffer.Sample(WrapSampler, Input.f2TexCoord).xyz;
    float3 Normal = NormalGBuffer.Sample(WrapSampler, Input.f2TexCoord).xyz;
    float3 AO_Metallic_Roughness = AO_Metallic_Roughtness_GBuffer.Sample(WrapSampler, Input.f2TexCoord).xyz;
    float3 Emissive = EmissiveGBuffer.Sample(WrapSampler, Input.f2TexCoord).xyz;
    float3 F0 = FresnelReflectanceGBuffer.Sample(WrapSampler, Input.f2TexCoord).xyz;

    float AmbientOcculusion = AO_Metallic_Roughness.x;
    float Metallic = AO_Metallic_Roughness.y;   
    float Roughness = AO_Metallic_Roughness.z;
    
    float3 ToEye = normalize(ViewPosition - Position);
    float3 ReflectDir = reflect(-ToEye, Normal);
        
    float3 IBLDiffuse = DiffuseTexture.Sample(WrapSampler, Normal).rgb;
    float3 IBLSpecular = SpecularTexture.Sample(WrapSampler, ReflectDir).rgb;
    
    float3 DiffuseIBL = IBLDiffuse * Diffuse;
    float3 SpecularIBL = IBLSpecular * Specular;
    
    float4 Result = float4(0.f, 0.f, 0.f, 0.f);
    Result += float4(CalculateIBL(F0, Diffuse, AmbientOcculusion, Metallic, Roughness, Normal, ToEye, SpecularTexture, DiffuseTexture, BRDFTexture, WrapSampler) + Emissive, 1.f) / 2.f;
    Result += float4((DiffuseIBL + SpecularIBL), 1.f) / 2.f;
    return Result;
}