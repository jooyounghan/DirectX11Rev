#include "BoundingObjectHeader.hlsli"

cbuffer CameraViewProj : register(b0)
{
    matrix ViewProjMatrix;
    matrix ViewProjInvMatrix;
    float3 ViewPosition;
    float Dummy;
};

cbuffer ModelMatrix : register(b1)
{
    matrix ModelMatrix;
    matrix ModelInvMatrix;
};

BoundingObjectVertexOutPut main(BoundingObjectVertexInput Input)
{
    BoundingObjectVertexOutPut Result;

    Result.f4ProjPos = mul(float4(Input.f3WorldPos, 1.f), ModelMatrix);
    Result.f4ProjPos = mul(Result.f4ProjPos, ViewProjMatrix);
    
    return Result;
}