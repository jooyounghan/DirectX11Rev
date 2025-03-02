#include "DebugComponentHeader.hlsli"

cbuffer ComponentBuffer : register(b0)
{
    uint IDValues;
    float3 dummy;
};

DebugComponentPixelOutPut main(DebugComponentVertexOutPut input)
{
    DebugComponentPixelOutPut result;
    result.f4Color = float4(0.f, 0.f, 0.f, 1.f);
    result.uiID = IDValues;
    return result;
}