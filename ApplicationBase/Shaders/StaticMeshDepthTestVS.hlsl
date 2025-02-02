#include "DepthTestHeader.hlsli"

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

float4 main(StaticMeshVertexInputForDepthTest Input) : SV_POSITION
{
    float4 f4ProjPos = mul(float4(Input.f3WorldPos, 1.f), ModelMatrix);
    f4ProjPos = mul(f4ProjPos, ViewProjMatrix);

    return f4ProjPos;
}