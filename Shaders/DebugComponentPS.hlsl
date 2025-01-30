#include "DebugComponentHeader.hlsli"

cbuffer ComponentBuffer : register(b0)
{
    uint IDValues;
    float3 DebugColor;
};

DebugComponentPixelOutPut main(DebugComponentVertexOutPut Input)
{
    DebugComponentPixelOutPut Result;
    Result.f4Color = float4(DebugColor, 1.f);
    Result.uiID = IDValues;
    return Result;
}