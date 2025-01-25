#include "MeshComponentHeader.hlsli"

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

MeshComponentVertexOutput main(SkeletalComponentVertexInput Input)
{
    MeshComponentVertexOutput Result;

    float d = distance(Input.f3WorldPos, ViewPosition);

    float MinTessDistance = 0.f;
    float MaxTessDistance = 1000.f;
    float MinTessFactor = 1.f;
    float MaxTessFactor = 10.f;
    
    //float tess = 1.f - saturate(d / (MaxTessDistance - MinTessDistance));
    float tess = lerp(MaxTessFactor, MinTessFactor, saturate(d / MaxTessDistance));
    
    Result.fTessFactor = 1.f;
    //Result.fTessFactor = tess;
    Result.fLODLevel = 2.f;
    
    float4 f4WorldNormalIn = float4(Input.f3WorldNormal, 0.f);
    float4 f4WorldTangentIn = float4(Input.f3WorldTangent, 0.f);
    float4 f4WorldPosIn = float4(Input.f3WorldPos, 1.f);
    
    float4 f4WorldNormalOut = 0;
    float4 f4WorldTangentOut = 0;
    float4 f4WorldPosOut = 0;
    
    [unroll]
    for (int i = 0; i < 4; ++i)
    {
        float weight = Input.f4BlendWeight[i];
        uint index = Input.f4BlendIndices[i];
        float4x4 boneTransform = BoneTransformation[index];
        
        f4WorldNormalOut += weight * mul(f4WorldNormalIn, boneTransform);
        f4WorldTangentOut += weight * mul(f4WorldTangentIn, boneTransform);
        f4WorldPosOut += weight * mul(f4WorldPosIn, boneTransform);
    }
    
    
    Result.f3WorldNormal = normalize(f4WorldNormalOut.xyz);
    Result.f3ModelTangent = normalize(mul(f4WorldTangentOut, ModelMatrix).xyz);

    Result.f2TexCoord = Input.f2TexCoord;
    Result.f4WorldjPos = f4WorldPosOut;

    return Result;
}