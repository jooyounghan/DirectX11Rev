#include "GBufferResolverHeader.hlsli"
#include "PBRHeader.hlsli"

cbuffer CameraViewConstantBuffer : register(b0)
{
    matrix viewProjMatrix;
    matrix viewProjInvMatrix;
    float3 viewPosition;
    float dummy;
};

TextureCube specularIBLTexture : register(t0);
TextureCube diffuseIBLTexture : register(t1);
Texture2D brdfLUTTexture : register(t2);

Texture2D positionGBuffer : register(t3);
Texture2D specularGBuffer : register(t4);
Texture2D diffuseGBuffer : register(t5);
Texture2D ao_metallic_roughtness_GBuffer : register(t6);
Texture2D normalGBuffer : register(t7);
Texture2D emissiveGBuffer : register(t8);

SamplerState wrapSampler : register(s0);
SamplerState clampSampler : register(s1);

float4 main(GBufferResolveVertexOutput input) : SV_TARGET
{
    float3 position = positionGBuffer.Sample(wrapSampler, input.f2TexCoord).xyz;
    float3 specular = specularGBuffer.Sample(wrapSampler, input.f2TexCoord).xyz;
    float3 diffuse = diffuseGBuffer.Sample(wrapSampler, input.f2TexCoord).xyz;
    float3 normal = normalGBuffer.Sample(wrapSampler, input.f2TexCoord).xyz;
    float3 ao_metallic_roughness = ao_metallic_roughtness_GBuffer.Sample(wrapSampler, input.f2TexCoord).xyz;
    float3 emissive = emissiveGBuffer.Sample(wrapSampler, input.f2TexCoord).xyz;

    float ambientOcculusion = ao_metallic_roughness.x;
    float metallic = ao_metallic_roughness.y;
    float roughness = ao_metallic_roughness.z;
    
    float3 toEye = normalize(viewPosition - position);
    float3 toLight = -reflect(toEye, normal);
    
    return float4(
        CalculateIBL(
            specular, diffuse, metallic, roughness, 
            normal, toLight, toEye,
            specularIBLTexture, diffuseIBLTexture, brdfLUTTexture, clampSampler
        ) * ambientOcculusion + emissive, 1.f
    );
}