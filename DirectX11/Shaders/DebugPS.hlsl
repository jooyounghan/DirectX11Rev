#include "DebugHeader.hlsli"

cbuffer PickingIDColor : register(b0)
{
    float4 PickingColor;
};

cbuffer DebuggingColor : register(b1)
{
    float4 Color;
};

float4 main(DebugVertexOutput Input) : SV_TARGET
{
    return Color;
}