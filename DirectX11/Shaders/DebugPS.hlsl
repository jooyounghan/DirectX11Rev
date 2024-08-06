#include "DebugHeader.hlsli"

float4 main(DebugVertexOutput Input) : SV_TARGET
{
    return float4(Input.f2TexCoord.x, 0.f, Input.f2TexCoord.y, 1.f);
}