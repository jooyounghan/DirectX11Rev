#include "DepthTestHeader.hlsli"

cbuffer CameraViewConstantBuffer : register(b0)
{
    matrix ViewProjMatrix;
    matrix ViewProjInvMatrix;
    float3 ViewPosition;
    float Dummy;
};

cbuffer ModelConstantBuffer : register(b1)
{
    matrix ModelMatrix;
    matrix ModelInvMatrix;
};

StructuredBuffer<float4x4> BoneTransformation : register(t0);

float4 main(SkeletalMeshVertexInputForDepthTest Input) : SV_POSITION
{
    float4 f4WorldPosIn = float4(Input.f3WorldPos, 1.f);
    float4 f4ProjPos = float4(0.f, 0.f, 0.f, 0.f);
    
    [unroll]
    for (int i = 0; i < 4; ++i)
    {
        float weight = Input.f4BlendWeight[i];
        uint index = Input.f4BlendIndices[i];
        float4x4 boneTransform = BoneTransformation[index];
        
        f4ProjPos += weight * mul(f4WorldPosIn, boneTransform);
    }
    f4ProjPos = mul(float4(Input.f3WorldPos, 1.f), ModelMatrix);
    f4ProjPos = mul(f4ProjPos, ViewProjMatrix);
    return f4ProjPos;
}