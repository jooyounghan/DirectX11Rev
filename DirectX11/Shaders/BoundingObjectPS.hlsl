#include "BoundingObjectHeader.hlsli"

cbuffer DebuggingColor : register(b0)
{
    float4 Color;
};

cbuffer PickingID : register(b1)
{
    float4 IDValues;
};

BoundingObjectPixelOutPut main(BoundingObjectVertexOutPut Input)
{
    BoundingObjectPixelOutPut Result;
    Result.f4Color = Color;
    Result.f4ID = IDValues;
    return Result;
}