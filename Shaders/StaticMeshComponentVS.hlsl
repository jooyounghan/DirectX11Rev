#include "MeshComponentHeader.hlsli"

cbuffer CameraViewConstantBuffer : register(b0)
{
    matrix viewProjMatrix;
    matrix viewProjInvMatrix;
    float3 viewPosition;
    float dummy;
};

MeshComponentVertexOutput main(StaticComponentVertexInput input)
{
    MeshComponentVertexOutput result;
       
    float d = distance(input.f3WorldPos, viewPosition);

    result.fTessFactor = 1.f;
    //Result.fTessFactor = getTessFactor(d);
    result.fLODLevel = 2.f;
    
    result.f3WorldNormal = input.f3WorldNormal;
    result.f2TexCoord = input.f2TexCoord;
    result.f4WorldjPos = float4(input.f3WorldPos, 1.f);

    return result;
}