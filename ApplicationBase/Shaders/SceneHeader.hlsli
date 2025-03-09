struct SceneVertexInput
{
    float3 f3WorldPos : POSITION;
    float2 f2TexCoord : TEXCOORD;
    float3 f3WorldNormal : NORMAL;
};

struct SceneVertexOutput
{
    float4 f4ProjPos : SV_Position;
    float2 f2TexCoord : TEXCOORD;
};

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