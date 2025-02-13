#include "GBufferResolverHeader.hlsli"

GBufferResolveVertexOutput main(GBufferResolveVertexInput input)
{
    GBufferResolveVertexOutput result;

    result.f4WorldjPos = float4(input.f3WorldPos, 1.f);
    result.f2TexCoord = input.f2TexCoord;
    return result;
}