#include "DepthTestHeader.hlsli"

cbuffer CameraViewConstantBuffer : register(b0)
{
    matrix viewProjMatrix;
    matrix viewProjInvMatrix;
    float3 viewPosition;
    float dummy;
};

cbuffer ModelConstantBuffer : register(b1)
{
    matrix modelMatrix;
    matrix modelInvMatrix;
};

float4 main(StaticMeshVertexInputForDepthTest input) : SV_POSITION
{
    float4 f4ProjPos = mul(float4(input.f3WorldPos, 1.f), modelMatrix);
    f4ProjPos = mul(f4ProjPos, viewProjMatrix);

    return f4ProjPos;
}