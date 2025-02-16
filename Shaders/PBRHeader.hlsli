float3 GetNormalFromMap(float3 N, float3 B, float3 T, float2 UV, Texture2D normalTexture, SamplerState samplerState)
{
    float3 normal = normalTexture.Sample(samplerState, UV).rgb;
    normal = 2.0 * normal - 1.0;
    float3x3 TBN = float3x3(T, B, N);
    return normalize(mul(normal, TBN));
}

float3 SchlickFresnelReflectTerm(float3 cSpecular, float LDotH)
{
    return cSpecular + (1.0 - cSpecular) * pow(1.0 - LDotH, 5.0);
}

float3 SchlickFresnelReflectRoughnessTerm(float3 cSpecular, float LDotH, float roughness)
{
    return cSpecular + (max(1.f - roughness, cSpecular) - cSpecular) * pow(1.0 - LDotH, 5.0);
}

float NormalDistributionGGXTerm(float NdotH, float roughness)
{
    float alpha2 = roughness * roughness;
    float d = (NdotH * NdotH) * (alpha2 - 1.0) + 1.0;
    return alpha2 / (3.141592 * d * d);
}

float SchlickG1(float VdotN, float halfRoughness)
{
    return VdotN / (VdotN * (1.0 - halfRoughness) + halfRoughness);
}

float SchlickGeometryModel(float LdotN, float VdotN, float roughness)
{
    float k = roughness / 2.0;
    return SchlickG1(LdotN, k) * SchlickG1(VdotN, k);
}

float3 GetIBLDiffuseTerm(float3 kd, float3 diffuseColor, float3 halfVector, TextureCube diffuseIBLTexture, SamplerState clampSampler)
{
    float3 diffuseIBL = diffuseIBLTexture.SampleLevel(clampSampler, halfVector, 0).rgb;
    return kd * diffuseColor * diffuseIBL;
}

float3 GetIBLSpecularTerm(float3 ks, float VDotH, float3 toLight, float roughness, TextureCube specularIBLTexture, Texture2D brdfLUTTextrure, SamplerState clampSampler)
{
    float2 brdfLUT = brdfLUTTextrure.SampleLevel(clampSampler, float2(VDotH, roughness), 0.0f).rg;
    float3 specularIBL = specularIBLTexture.SampleLevel(clampSampler, toLight, roughness * 5.0f).rgb;
    return (ks * brdfLUT.x + brdfLUT.y) * specularIBL;
}

float3 CalculateIBL(
    float3 kd, float3 ks,
    float3 diffuseColor,
    float metallic, float roughness, 
    float3 halfVector, float3 toLight, float VDotH,
    TextureCube specularIBLTexture, TextureCube diffuseIBLTexture,
    Texture2D brdfLUTTextrure, SamplerState clampSampler
)
{
    float3 diffuseTerm = GetIBLDiffuseTerm(kd, diffuseColor, halfVector, diffuseIBLTexture, clampSampler);
    float3 specularTerm = GetIBLSpecularTerm(ks, VDotH, toLight, roughness, specularIBLTexture, brdfLUTTextrure, clampSampler);
    return (diffuseTerm + specularTerm);
}