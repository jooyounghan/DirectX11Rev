#include "EnvironmentActorHeader.hlsli"

cbuffer ToneMapping : register(b0)
{
    float Exposure;
    float Gamma;
    float2 Dummy;
};

Texture2D HDRTexture : register(t0);

SamplerState WrapSampler : register(s0);

float4 main(EnvironmentActorVertexOutput Input) : SV_TARGET
{
    float4 Color = clamp(Exposure * HDRTexture.Sample(WrapSampler, Input.f2TexCoord), 0.f, 1.f);
    Color = pow(Color, 1.f / Gamma);

    return Color;
}