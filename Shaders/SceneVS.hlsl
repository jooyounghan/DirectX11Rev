#include "SceneHeader.hlsli"

cbuffer CameraViewProj : register(b0)
{
    matrix ViewProjMatrix;
    matrix ViewProjInvMatrix;
    float3 ViewPosition;
    float Dummy;
};


SceneVertexOutput main(SceneVertexInput Input)
{
    SceneVertexOutput Result;
    Result.f4ProjPos = mul(float4(Input.f3WorldPos, 1.f), ViewProjMatrix);
    Result.f2TexCoord = Input.f2TexCoord;
    return Result;
}