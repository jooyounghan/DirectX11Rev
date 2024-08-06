#include "DebugHeader.hlsli"

cbuffer DebuggingColor : register(b0)
{
    float4 Color;
};

float4 main(DebugVertexOutput Input) : SV_TARGET
{
    return Color;
}