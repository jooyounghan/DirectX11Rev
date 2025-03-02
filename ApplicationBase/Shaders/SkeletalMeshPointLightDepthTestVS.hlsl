#include "DepthTestHeader.hlsli"
#include "DirectLightHeader.hlsli"

cbuffer ModelConstantBuffer : register(b0)
{
    matrix modelMatrix;
    matrix modelInvMatrix;
};

cbuffer PointLightViewEntity : register(b1)
{
    matrix viewProjMatrix;
    matrix viewProjInvMatrix;
    float3 viewPosition;
    float dummy;
};

StructuredBuffer<float4x4> boneTransformation : register(t0);

float4 main(SkeletalMeshVertexInputForDepthTest input) : SV_POSITION
{
    float4 f4WorldPosIn = float4(input.f3WorldPos, 1.f);
    float4 f4ProjPos = float4(0.f, 0.f, 0.f, 0.f);
    
    [unroll]
    for (int i = 0; i < 4; ++i)
    {
        float weight = input.f4BlendWeight[i];
        uint index = input.f4BlendIndices[i];
        float4x4 boneTransform = boneTransformation[index];
        
        f4ProjPos += weight * mul(f4WorldPosIn, boneTransform);
    }
    f4ProjPos = mul(float4(input.f3WorldPos, 1.f), modelMatrix);
    f4ProjPos = mul(f4ProjPos, viewProjMatrix);
    return f4ProjPos;
}