#include "BoundingComponentHeader.hlsli"

cbuffer BoundingComponentColor : register(b0)
{
    float4 Color;
};

cbuffer PickingID : register(b1)
{
    float4 IDValues;
};

BoundingComponentPixelOutPut main(BoundingComponentVertexOutPut Input)
{
    BoundingComponentPixelOutPut Result;
    Result.f4Color = Color;
    Result.f4ID = IDValues;
    return Result;
}