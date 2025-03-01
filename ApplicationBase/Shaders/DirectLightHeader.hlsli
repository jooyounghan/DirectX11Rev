#include "PBRHeader.hlsli"

static const uint MaxSpotLightCount = 1024;
static const uint MaxPointLightCount = 1024;
static const uint DefaultNearZ = 0.1f;

struct LightEntity
{
    float lightPower;
    float fallOffStart;
    float fallOffEnd;
    float spotPower;
};

struct LightViewEntity
{
    matrix viewProjMatrix;
    matrix viewProjInvMatrix;
    float3 viewPosition;
    float dummy;
};

float3 GetDirectSpotLighted(
    LightEntity spotLightEntitiy,
    LightViewEntity spotLightViewEntity,
    float4 f4ModelPos,
    float3 toEye,
    float3 normal,
    Texture2DArray spotLightShadowMaps,
    uint spotLightIdx,
    SamplerState wrapSampler,
    float3 kd,
    float3 ks,
    float3 diffuse,
    float roughness
)
{
    float4 lightViewProjPos = mul(f4ModelPos, spotLightViewEntity.viewProjMatrix);
    lightViewProjPos /= lightViewProjPos.w;
    float2 lightUV = lightViewProjPos.xy;
    lightUV.x = lightUV.x / 2.f + 0.5f;
    lightUV.y = 0.5f - lightUV.y / 2.f;
        
    if (lightViewProjPos.w <= 0 || lightUV.x <= 0.f || lightUV.y <= 0.f || lightUV.x >= 1.f || lightUV.y >= 1.f)
        return float3(0.f, 0.f, 0.f);
                
    float shadowDepth = spotLightShadowMaps.Sample(wrapSampler, float3(lightUV, spotLightIdx)).x;
    if (lightViewProjPos.z > shadowDepth + 1E-5)
        return float3(0.f, 0.f, 0.f);
        
    float3 spotLightPos = spotLightViewEntity.viewPosition;
    float3 toSpotLight = spotLightPos - f4ModelPos.xyz;
    float lightPower = spotLightEntitiy.lightPower;
    float fallOffStart = spotLightEntitiy.fallOffStart;
    float fallOffEnd = spotLightEntitiy.fallOffEnd;
        
    float distToLight = length(toSpotLight);
    lightPower *= clamp((distToLight - fallOffEnd) / (fallOffStart - fallOffEnd), 0.0f, 1.0f);
    lightPower *= max(0.f, 1 - dot(lightViewProjPos.xy, lightViewProjPos.xy));
    if (lightPower < 1E-6)
        return float3(0.f, 0.f, 0.f);
    
    toSpotLight = toSpotLight / distToLight;
               
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
            
    return (directDiffuseBRDF + directSpecularBRDF) * max(0.f, pow(spotLDotN * lightPower, spotLightEntitiy.spotPower));
}