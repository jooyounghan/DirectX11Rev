#include "PositionOnlyPathHeader.hlsli"

cbuffer DebuggingColor : register(b0)
{
    float4 Color;
};

float4 main(PositionOnlyVertexOutPut Input) : SV_TARGET
{
    return Color;
}