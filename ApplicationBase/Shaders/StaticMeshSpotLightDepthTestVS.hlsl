#include "DepthTestHeader.hlsli"
#include "DirectLightHeader.hlsli"

cbuffer ComponentBuffer : register(b0)
{
    uint IDValues;
    uint lightIndex;
    float2 dummy;
};

cbuffer ModelConstantBuffer : register(b1)
{
    matrix modelMatrix;
    matrix modelInvMatrix;
};

StructuredBuffer<LightViewEntity> lightViewEntities : register(t0);

float4 main(StaticMeshVertexInputForDepthTest input) : SV_POSITION
{
    float4 f4ProjPos = mul(float4(input.f3WorldPos, 1.f), modelMatrix);
    f4ProjPos = mul(f4ProjPos, lightViewEntities[lightIndex].viewProjMatrix);

    return f4ProjPos;
}