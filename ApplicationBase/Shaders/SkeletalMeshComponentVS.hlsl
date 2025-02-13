#include "MeshComponentHeader.hlsli"

cbuffer CameraViewConstantBuffer : register(b0)
{
    matrix viewProjMatrix;
    matrix viewProjInvMatrix;
    float3 viewPosition;
    float dummy;
};

StructuredBuffer<float4x4> boneTransformation : register(t0);

MeshComponentVertexOutput main(SkeletalComponentVertexInput input)
{
    MeshComponentVertexOutput result;

    float d = distance(input.f3WorldPos, viewPosition);
     
    result.fTessFactor = 1.f;
    //Result.fTessFactor = getTessFactor(d);
    result.fLODLevel = 2.f;
    
    float4 f4WorldNormalIn = float4(input.f3WorldNormal, 0.f);
    float4 f4WorldPosIn = float4(input.f3WorldPos, 1.f);
    
    float4 f4WorldNormalOut = 0;
    float4 f4WorldTangentOut = 0;
    float4 f4WorldPosOut = 0;
    
    [unroll]
    for (int i = 0; i < 4; ++i)
    {
        float weight = input.f4BlendWeight[i];
        uint index = input.f4BlendIndices[i];
        float4x4 boneTransform = boneTransformation[index];
        
        f4WorldNormalOut += weight * mul(f4WorldNormalIn, boneTransform);
        f4WorldPosOut += weight * mul(f4WorldPosIn, boneTransform);
    }
    
    
    result.f3WorldNormal = normalize(f4WorldNormalOut.xyz);
    result.f2TexCoord = input.f2TexCoord;
    result.f4WorldjPos = f4WorldPosOut;

    return result;
}