#include "PostProcessHeader.hlsli"
#include "PBRHeader.hlsli"

cbuffer CameraEntityBuffer : register(b0)
{
    uint width;
    uint height;
    float focusLength;
    float aperture;
    float nearZ;
    float farZ;
    float2 cameraEntityDummy;
};

Texture2D filmTexture : register(t0);
Texture2D depthTexture : register(t1);

SamplerState wrapSampler : register(s0);
SamplerState clampSampler : register(s1);

float4 main(PostProcessVertexOutput input) : SV_TARGET
{
    float2 uv = input.f2TexCoord;

    float depth = depthTexture.Sample(wrapSampler, uv).r;
    float distance = NDCToViewDepth(depth, nearZ, farZ);
    
    float coc = ComputeCoC(distance, focusLength, aperture);
    float blurRadius = coc * 5.f;

    float3 blurredColor = GetGaussianBluredSample16(filmTexture, clampSampler, uv, float2(1.f / (float) width, 1.f / (float) height), blurRadius);
    float3 originalColor = filmTexture.Sample(wrapSampler, uv).rgb;

    return float4(lerp(originalColor, blurredColor, coc), 1.0);
}