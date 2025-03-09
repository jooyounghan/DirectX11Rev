float NDCToViewDepth(float zNDC, float nearZ, float farZ)
{
    return nearZ * farZ / (farZ - zNDC * (farZ - nearZ));
}

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

