#include "SkeletalHeader.hlsli"

Texture2D TestTexture : register(t0);
TextureCube CubeTexture : register(t1);

SamplerState WrapSampler : register(s0);
SamplerState ClampSampler : register(s1);

float4 main(BasicVertexOutput Input) : SV_TARGET
{
    return CubeTexture.Sample(WrapSampler, Input.f3ModelNormal);
}