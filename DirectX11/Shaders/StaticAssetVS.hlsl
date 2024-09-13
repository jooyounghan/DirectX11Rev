#include "StaticAssetHeader.hlsli"

cbuffer ModelMatrix : register(b0)
{
    matrix ModelMatrix;
    matrix ModelInvMatrix;
};

cbuffer CameraViewProj : register(b1)
{
    matrix ViewProjMatrix;
    matrix ViewProjInvMatrix;
};

BasicVertexOutput main(BasicVertexInput Input)
{
    BasicVertexOutput Result;
       
    //Result.f3ModelNormal = normalize(mul(float4(Input.f3WorldNormal, 0.f), ModelInvMatrix)).xyz;
    Result.f3ModelNormal = Input.f3WorldNormal;

    Result.f4ProjPos = mul(float4(Input.f3WorldPos, 1.f), ModelMatrix);    
    Result.f4ProjPos = mul(Result.f4ProjPos, ViewProjMatrix);
    Result.f2TexCoord = Input.f2TexCoord;
    
    return Result;
}