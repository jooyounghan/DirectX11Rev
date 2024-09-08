#include "SkeletalHeader.hlsli"

Texture2D TestTexture : register(t0);

SamplerState WrapSampler : register(s0);
SamplerState ClampSampler : register(s1);

float4 main(BasicVertexOutput Input) : SV_TARGET
{
    return TestTexture.Sample(WrapSampler, Input.f2TexCoord);
}