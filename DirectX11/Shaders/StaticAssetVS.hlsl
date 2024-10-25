#include "StaticAssetHeader.hlsli"

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


BasicVertexOutput main(BasicVertexInput Input)
{
    BasicVertexOutput Result;
       
    Result.f3ModelNormal = normalize(mul(float4(Input.f3WorldNormal, 0.f), ModelInvMatrix).xyz);
    Result.f3ModelTangent = normalize(mul(float4(Input.f3WorldTangent, 0.f), ModelMatrix).xyz);
    Result.f3ModelBitangent = normalize(mul(float4(Input.f3WorldBitangent, 0.f), ModelMatrix).xyz);
    
    Result.f4ProjPos = mul(float4(Input.f3WorldPos, 1.f), ModelMatrix);    
    Result.f4ProjPos = mul(Result.f4ProjPos, ViewProjMatrix);
    Result.f2TexCoord = Input.f2TexCoord;
    
    return Result;
}