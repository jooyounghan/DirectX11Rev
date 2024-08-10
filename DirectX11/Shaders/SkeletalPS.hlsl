#include "SkeletalHeader.hlsli"

cbuffer PickingIDColor : register(b0)
{
    float4 PickingColor;
};

float4 main(BasicVertexOutput Input) : SV_TARGET
{
    return float4(PickingColor.x * Input.f3ModelNormal.x, PickingColor.y * Input.f3ModelNormal.y, PickingColor.z * Input.f3ModelNormal.z, 1.f);
}