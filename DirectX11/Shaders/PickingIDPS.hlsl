#include "PositionOnlyPathHeader.hlsli"

cbuffer PickingID : register(b0)
{
    float4 IDValues;
};

float4 main() : SV_TARGET
{
	return IDValues;
}