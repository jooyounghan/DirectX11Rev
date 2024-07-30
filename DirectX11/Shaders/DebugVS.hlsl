#include "DebugHeader.hlsli"

cbuffer CameraViewProj : register(b0)
{
    matrix ViewProjMatrix;
    matrix ViewProjInvMatrix;
};

cbuffer ModelMatrix : register(b1)
{
    matrix ModelMatrix;
    matrix ModelInvMatrix;
};

DebugVertexOutput main(DebugVertexInput Input)
{
    DebugVertexOutput Result;

    Result.f4ProjPos = mul(float4(Input.f3WorldPos, 1.f), ModelMatrix);
    Result.f4ProjPos = mul(Result.f4ProjPos, ViewProjMatrix);
    Result.f2TexCoord = Input.f2TexCoord;
    
    return Result;
}