#include "EnvironmentActorHeader.hlsli"

Texture2D HDRTexture : register(t0);

SamplerState WrapSampler : register(s0);

float4 main(EnvironmentActorVertexOutput Input) : SV_TARGET
{
    return HDRTexture.Sample(WrapSampler, Input.f2TexCoord);
}