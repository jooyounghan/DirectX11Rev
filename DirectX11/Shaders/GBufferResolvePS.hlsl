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
Texture2D BaseColorGBuffer : register(t4);
Texture2D NormalGBuffer : register(t5);
Texture2D AO_Metallic_Roughtness_GBuffer : register(t6);
Texture2D EmissiveGBuffer : register(t7);

SamplerState WrapSampler : register(s0);

float4 main(GBufferResolveVertexOutput Input) : SV_TARGET
{
    float3 Position = PositionGBuffer.Sample(WrapSampler, Input.f2TexCoord).xyz;
    float3 BaseColor = BaseColorGBuffer.Sample(WrapSampler, Input.f2TexCoord).xyz;
    float3 Normal = NormalGBuffer.Sample(WrapSampler, Input.f2TexCoord).xyz;
    float3 AO_Metallic_Roughness = AO_Metallic_Roughtness_GBuffer.Sample(WrapSampler, Input.f2TexCoord).xyz;
    float3 Emissive = EmissiveGBuffer.Sample(WrapSampler, Input.f2TexCoord).xyz;
    
    float3 ToEye = normalize(ViewPosition - Position);
    float AO = AO_Metallic_Roughness.x;
    float Metallic = AO_Metallic_Roughness.y;   
    float Roughness = AO_Metallic_Roughness.z;
    
    return float4(
        CalculateIBL(float3(0.04, 0.04, 0.04), BaseColor, AO, Metallic, Roughness, Normal,
            ToEye, SpecularTexture, DiffuseTexture, BRDFTexture, WrapSampler)
        + Emissive, 1.f
    );
}