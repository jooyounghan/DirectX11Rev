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
    
    // Direct Lighting Test ==============================================================
    float3 spotLightPos = float3(0.f, 0.f, 0.f);
    float3 toSpotLight = normalize(spotLightPos - position);
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
    
    return float4(color, 1.f);
}