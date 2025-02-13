#include "DebugComponentHeader.hlsli"

cbuffer CameraViewProj : register(b0)
{
    matrix viewProjMatrix;
    matrix viewProjInvMatrix;
    float3 viewPosition;
    float dummy;
};

cbuffer ModelMatrix : register(b1)
{
    matrix modelMatrix;
    matrix modelInvMatrix;
};

DebugComponentVertexOutPut main(DebugComponentVertexInput input)
{
    DebugComponentVertexOutPut result;

    result.f4ProjPos = mul(float4(input.f3WorldPos, 1.f), modelMatrix);
    result.f4ProjPos = mul(result.f4ProjPos, viewProjMatrix);
    
    return result;
}