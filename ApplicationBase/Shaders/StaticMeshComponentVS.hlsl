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


MeshComponentVertexOutput main(StaticComponentVertexInput Input)
{
    MeshComponentVertexOutput Result;
       
    float d = distance(Input.f3WorldPos, ViewPosition);

    float MinTessDistance = 0.f;
    float MaxTessDistance = 1000.f;
    float MinTessFactor = 0.f;
    float MaxTessFactor = 3.f;
    //float tess = 1.f - saturate(d / (MaxTessDistance - MinTessDistance));
    float tess = lerp(MaxTessFactor, MinTessFactor, saturate(d / MaxTessDistance));
    
    Result.fTessFactor = 1.f;
    //Result.fTessFactor = tess;
    Result.fLODLevel = 2.f;
    
    Result.f3WorldNormal = Input.f3WorldNormal;
    Result.f2TexCoord = Input.f2TexCoord;
    Result.f4WorldjPos = float4(Input.f3WorldPos, 1.f);

    return Result;
}