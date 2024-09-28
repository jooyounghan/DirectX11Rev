#include "EnvironmentActorHeader.hlsli"

Texture2D HDRTexture : register(t0);

cbuffer ToneMapping : register(b0)
{
    float Exposure;
    float Gamma;
    float2 Dummy;
};

SamplerState WrapSampler : register(s0);

float4 main(EnvironmentActorVertexOutput Input) : SV_TARGET
{
    float4 Color = clamp(Exposure * HDRTexture.Sample(WrapSampler, Input.f2TexCoord), 0.f, 1.f);
    Color = pow(Color, 1.f / Gamma);
    return Color;
}