#include "SkeletalHeader.hlsli"

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


BasicVertexOutput main(BasicVertexInput Input)
{
    BasicVertexOutput Result;

    Result.f3ModelNormal = normalize(mul(float4(Input.f3WorldNormal, 0.f), ModelInvMatrix)).xyz;

    Result.f4ProjPos = mul(float4(Input.f3WorldPos, 1.f), ModelMatrix);
    Result.f4ProjPos = mul(Result.f4ProjPos, ViewProjMatrix);
    Result.f2TexCoord = Input.f2TexCoord;
    
    return Result;
}