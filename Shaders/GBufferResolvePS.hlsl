#include "GBufferResolverHeader.hlsli"
#include "DirectLightHeader.hlsli"

cbuffer CameraViewConstantBuffer : register(b0)
{
    matrix viewProjMatrix;
    matrix viewProjInvMatrix;
    float3 viewPosition;
    float dummy;
};

cbuffer LightManagerEntityBuffer : register(b1)
{
    uint spotLightCount;
    uint pointLightCount;
    uint2 b2Dummy;
}

TextureCube specularIBLTexture : register(t0);
TextureCube diffuseIBLTexture : register(t1);
Texture2D brdfLUTTexture : register(t2);

Texture2D positionGBuffer : register(t3);
Texture2D specularGBuffer : register(t4);
Texture2D diffuseGBuffer : register(t5);
Texture2D ao_metallic_roughtness_GBuffer : register(t6);
Texture2D normalGBuffer : register(t7);
Texture2D emissiveGBuffer : register(t8);

StructuredBuffer<LightEntity> spotLightEntities : register(t9);
StructuredBuffer<LightViewEntity> spotLightViewEntities : register(t10);
Texture2DArray spotLightShadowMaps : register(t11);

StructuredBuffer<LightEntity> pointLightEntities : register(t12);
StructuredBuffer<LightPosition> pointLightPositions : register(t13);
TextureCubeArray pointLightShadowMaps : register(t14);


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
    
    float VDotH = max(dot(normal, toEye), 0.f);
    
    float3 f0 = lerp(specular, diffuse, metallic);
    float3 ks = SchlickFresnelReflectRoughnessTerm(f0, max(0.0, dot(toLight, normal)), roughness);
    float3 kd = (1.0f - ks) * (1 - metallic);
    
    float3 color = float3(
        CalculateIBL(
            kd, ks, diffuse, metallic, roughness,
            normal, toLight, VDotH,
            specularIBLTexture, diffuseIBLTexture, brdfLUTTexture, clampSampler
        ) * ambientOcculusion + emissive
    );
    
    
    float4 f4Position = float4(position, 1.f);
    for (uint spotLightIdx = 0; spotLightIdx < spotLightCount; ++spotLightIdx)
    {
        color += GetDirectSpotLighted(
            spotLightEntities, spotLightViewEntities,
            f4Position, toEye, normal,
            spotLightShadowMaps, spotLightIdx, wrapSampler,
            kd, ks, diffuse, roughness
        );
    }
       
    for (uint pointLightIdx = 0; pointLightIdx < 1; ++pointLightIdx)
    {
        color += GetDirectPointLighted(
            pointLightEntities, pointLightPositions,
            position, toEye, normal,
            pointLightShadowMaps, pointLightIdx, wrapSampler,
            kd, ks, diffuse, roughness
        );
  

      

        
        ;
    }
    
    return float4(color, 1.f);
}