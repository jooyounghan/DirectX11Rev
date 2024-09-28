#include "SkeletalAssetHeader.hlsli"

TextureCube SpecularTexture : register(t0);
TextureCube DiffuseTexture : register(t1);
Texture2D BRDFTexture : register(t2);

SamplerState WrapSampler : register(s0);
SamplerState ClampSampler : register(s1);

float4 main(BasicVertexOutput Input) : SV_TARGET
{
    return SpecularTexture.Sample(WrapSampler, Input.f3ModelNormal);
}