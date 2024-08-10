#include "StaticHeader.hlsli"

cbuffer PickingIDColor : register(b0)
{
    float4 PickingColor;
};

float4 main(BasicVertexOutput Input) : SV_TARGET
{
    return float4(Input.f3ModelNormal.x, Input.f3ModelNormal.y, Input.f3ModelNormal.z, 1.f);
}