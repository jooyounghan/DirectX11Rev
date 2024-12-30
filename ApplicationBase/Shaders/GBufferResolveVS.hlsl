#include "GBufferResolverHeader.hlsli"

GBufferResolveVertexOutput main(GBufferResolveVertexInput Input)
{
    GBufferResolveVertexOutput Result;

    Result.f4WorldjPos = float4(Input.f3WorldPos, 1.f);
    Result.f2TexCoord = Input.f2TexCoord;
    return Result;
}