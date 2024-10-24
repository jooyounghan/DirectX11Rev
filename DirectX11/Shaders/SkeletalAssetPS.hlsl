#include "SkeletalAssetHeader.hlsli"
#include "MaterialHeader.hlsli"

TextureCube SpecularTexture : register(t0);
TextureCube DiffuseTexture : register(t1);
Texture2D BRDFTexture : register(t2);

Texture2D MaterialTexture[8] : register(t3);

SamplerState WrapSampler : register(s0);
SamplerState ClampSampler : register(s1);

float4 main(BasicVertexOutput Input) : SV_TARGET
{
    return MaterialTexture[DIFFUSE_IDX].Sample(WrapSampler, float2(Input.f2TexCoord.x, Input.f2TexCoord.y));
}