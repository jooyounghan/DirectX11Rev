#include "BoundingObjectHeader.hlsli"

cbuffer DebuggingColor : register(b0)
{
    float4 Color;
};

cbuffer PickingID : register(b1)
{
    float4 IDValues;
};


DefferedBoundingObjectPixelOutput main(BoundingObjectVertexOutPut Input)
{
    DefferedBoundingObjectPixelOutput Result;
    Result.f4BaseColor = Color;
    Result.f4Normal = float4(1.f, 1.f, 1.f, 1.f);
    Result.AO_Metallic_Roughness = float4(0.f, 0.f, 0.f, 1.f);
    Result.Emissive = float4(0.f, 0.f, 0.f, 1.f);
    Result.f4ID = IDValues;
    return Result;
}