#include "PBRHeader.hlsli"

static const uint MaxSpotLightCount = 1024;
static const uint MaxPointLightCount = 1024;
static const float DefaultNearZ = 0.1f;

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

struct LightPosition
{
    float3 f3LightPos;
    float dummy;
};

float3 GetCubeMapFaceNormal(float3 dir)
{
    float3 absDir = abs(dir);
    float3 signDir = sign(dir);

    bool isX = (absDir.x >= absDir.y) && (absDir.x >= absDir.z);
    bool isY = (absDir.y > absDir.x) && (absDir.y >= absDir.z);
    
    return isX ? float3(signDir.x, 0, 0) :
           isY ? float3(0, signDir.y, 0) :
                 float3(0, 0, signDir.z);
}

float3 GetDirectSpotLighted(
    StructuredBuffer<LightEntity> spotLightEntities,
    StructuredBuffer<LightViewEntity> spotLightViewEntities,
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
    LightEntity spotLightEntitiy = spotLightEntities[spotLightIdx];
    LightViewEntity spotLightViewEntity = spotLightViewEntities[spotLightIdx];

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

float3 GetDirectPointLighted(
    StructuredBuffer<LightEntity> pointLightEntities ,
    StructuredBuffer<LightPosition> pointLightPositions,
    float3 f3ModelPos,
    float3 toEye,
    float3 normal,
    TextureCubeArray pointLightShadowMaps,
    uint pointLightIdx,
    SamplerState wrapSampler,
    float3 kd,
    float3 ks,
    float3 diffuse,
    float roughness
)
{
    LightEntity pointLightEntity = pointLightEntities[pointLightIdx];
    LightPosition pointLightPosition = pointLightPositions[pointLightIdx];

    float farZ = pointLightEntity.fallOffEnd;
    float3 fromLight = f3ModelPos - pointLightPosition.f3LightPos;
    float fromLightDistance = length(fromLight);
    fromLight = fromLight / fromLightDistance;
        
    float depthNDC = pointLightShadowMaps.Sample(wrapSampler, float4(fromLight, pointLightIdx)).x;
    float viewDepthZ = NDCToViewDepth(depthNDC, DefaultNearZ, farZ);

    float3 faceNormal = GetCubeMapFaceNormal(fromLight);
    float viewDistance = fromLightDistance * dot(faceNormal, fromLight);
    
    if (viewDistance > viewDepthZ + 0.1f)
        return float3(0.f, 0.f, 0.f);
                
    float lightPower = pointLightEntity.lightPower;
    float fallOffStart = pointLightEntity.fallOffStart;
    float fallOffEnd = pointLightEntity.fallOffEnd;
    float3 toPointLight = -fromLight;
        
    lightPower *= clamp((fromLightDistance - fallOffEnd) / (fallOffStart - fallOffEnd), 0.0f, 1.0f);
    if (lightPower < 1E-6)
        return float3(0.f, 0.f, 0.f);

    float3 pointHalfVector = (toEye + toPointLight) / 2.f;
    float pointLDotH = max(0.f, dot(toPointLight, pointHalfVector));
    float pointNDotH = max(0.f, dot(normal, pointHalfVector));
    float pointLDotN = max(0.f, dot(toPointLight, normal));
    float pointVDotN = max(0.f, dot(toEye, normal));
        
    float3 directDiffuseBRDF = (kd * diffuse / 3.141592);
    float3 directSpecularBRDF = SchlickFresnelReflectRoughnessTerm(ks, pointLDotH, roughness)
        * SchlickGeometryModel(pointLDotN, pointVDotN, roughness)
        * NormalDistributionGGXTerm(pointNDotH, roughness)
        / max((4 * pointLDotN * pointVDotN), 1E-6);
    
    return (directDiffuseBRDF + directSpecularBRDF) * max(0.f, pointLDotN * lightPower);
}