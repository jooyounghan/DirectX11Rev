#include "MeshObjectHeader.hlsli"

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

MeshObjectVertexOutput main(SkeletalVertexInput Input)
{
    MeshObjectVertexOutput Result;

    float d = distance(Input.f3WorldPos, ViewPosition);

    float MinTessDistance = 0.f;
    float MaxTessDistance = 1000.f;
    float MinTessFactor = 1.f;
    float MaxTessFactor = 10.f;
    float tess = 1.f - saturate(d / (MaxTessDistance - MinTessDistance));
    
    Result.fTessFactor = 2.f;
    Result.fLODLevel = 2.f;
    
    
    float Weights[4];
    Weights[0] = Input.f4BlendWeight.x;
    Weights[1] = Input.f4BlendWeight.y;
    Weights[2] = Input.f4BlendWeight.z;
    Weights[3] = Input.f4BlendWeight.w;
    
    uint Indices[4];
    Indices[0] = Input.f4BlendIndices.x;
    Indices[1] = Input.f4BlendIndices.y;
    Indices[2] = Input.f4BlendIndices.z;
    Indices[3] = Input.f4BlendIndices.w;
    
    
    float4 f4WorldNormalIn = float4(Input.f3WorldNormal, 0.f);
    float4 f4WorldTangentIn = float4(Input.f3WorldTangent, 0.f);
    float4 f4WorldPosIn = float4(Input.f3WorldPos, 1.f);
    
    float4 f4WorldNormalOut = float4(0.f, 0.f, 0.f, 0.f);
    float4 f4WorldTangentOut = float4(0.f, 0.f, 0.f, 0.f);
    float4 f4WorldPosOut = float4(0.f, 0.f, 0.f, 0.f);
    
    [unroll]
    for (int i = 0; i < 4; ++i)
    {
        f4WorldNormalOut += Weights[i] * mul(f4WorldNormalIn, BoneTransformation[Indices[i]]);
        f4WorldTangentOut += Weights[i] * mul(f4WorldTangentIn, BoneTransformation[Indices[i]]);
        f4WorldPosOut += Weights[i] * mul(f4WorldPosIn, BoneTransformation[Indices[i]]);
    }
    
    
    Result.f3ModelNormal = normalize(mul(f4WorldNormalOut, ModelInvMatrix).xyz);
    Result.f3ModelTangent = normalize(mul(f4WorldTangentOut, ModelMatrix).xyz);
    Result.f2TexCoord = Input.f2TexCoord;
    
    Result.f4ProjPos = mul(f4WorldPosOut, ModelMatrix);
    Result.f4ProjPos = mul(Result.f4ProjPos, ViewProjMatrix);
    
    return Result;
}