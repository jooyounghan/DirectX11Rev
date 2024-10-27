float3 GetNormalFromMap(float3 N, float3 B, float3 T, float LOD, float2 UV, Texture2D NormalTexture, SamplerState Sampler)
{
    float3 Normal = NormalTexture.SampleLevel(Sampler, UV, LOD).rgb;
    Normal = 2.0 * Normal - 1.0;
    float3x3 TBN = float3x3(T, B, N);
    return normalize(mul(Normal, TBN));
}

float3 SchlickFresnelReflectCoeffient(float3 F0, float CosTheta)
{
    return F0 + (1.0 - F0) * pow(1.0 - CosTheta, 5.0);
}

float NormalDistributionGGX(float NdotH, float Roughness)
{
    float alpha = Roughness * Roughness;
    float alpha2 = alpha * alpha;
    float d = (NdotH * NdotH) * (alpha2 - 1.0) + 1.0;
    return alpha2 / (3.141592 * d * d);
}

float SchlickG1(float NdotV, float k)
{
    return NdotV / (NdotV * (1.0 - k) + k);
}

float SchlickGeometryModel(float NdotL, float NdotE, float Roughness)
{
    //float r = Roughness + 1.0;
    //float k = (r * r) / 8.0;
    
    float alpha = Roughness * Roughness;
    float k = alpha / 2.0;
    
    return SchlickG1(NdotL, k) * SchlickG1(NdotE, k);
}

float3 DiffuseIBL(float3 F0, float3 BaseColor, float3 Normal, float Metallic, float3 ToEye, TextureCube DiffuseTexture, SamplerState WrapSampler)
{

    float3 F = SchlickFresnelReflectCoeffient(F0, max(0.0, dot(Normal, ToEye)));
    float3 kd = lerp(1.0 - F, 0.0, Metallic);
    float3 Irradiance = DiffuseTexture.SampleLevel(WrapSampler, Normal, 0).rgb;
    
    return kd * BaseColor * Irradiance;
}

float3 SpecularIBL(float3 F0, float3 BaseColor, float3 Normal, float Metallic, float Roughness, float3 ToEye, TextureCube SpecularTexture, Texture2D BRDFTextrure, SamplerState WrapSampler)
{
    float2 specularBRDF = BRDFTextrure.SampleLevel(WrapSampler, float2(dot(Normal, ToEye), 1.0 - Roughness), 0.0f).rg;
    float3 specularIrradiance = SpecularTexture.SampleLevel(WrapSampler, Normal, Roughness * 5.0f).rgb;
    
    return (F0 * specularBRDF.x + specularBRDF.y) * specularIrradiance;
}

float3 CalculateIBL(
    float3 FrenelConstant, float3 BaseColor, float AmbientOcculusion, 
    float Metallic, float Roughness, 
    float3 Normal, float3 ToEye,
    TextureCube SpecularTexture, TextureCube DiffuseTexture,
    Texture2D BRDFTextrure, SamplerState WrapSampler
)
{
    float3 F0 = lerp(FrenelConstant, BaseColor, Metallic);
    float3 DiffuseTerm = DiffuseIBL(F0, BaseColor, Normal, Metallic, ToEye, DiffuseTexture, WrapSampler);
    float3 SpecularTerm = SpecularIBL(F0, BaseColor, Normal, Metallic, Roughness, ToEye, SpecularTexture, BRDFTextrure, WrapSampler);
    return (DiffuseTerm + SpecularTerm) * AmbientOcculusion;
}