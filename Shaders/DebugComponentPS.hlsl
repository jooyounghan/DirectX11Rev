#include "DebugComponentHeader.hlsli"

cbuffer ComponentBuffer : register(b0)
{
    uint IDValues;
    float3 debugColor;
};

DebugComponentPixelOutPut main(DebugComponentVertexOutPut input)
{
    DebugComponentPixelOutPut result;
    result.f4Color = float4(debugColor, 1.f);
    result.uiID = IDValues;
    return result;
}