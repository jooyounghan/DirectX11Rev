#include "MeshObjectHeader.hlsli"

cbuffer CameraViewConstantBuffer : register(b0)
{
    matrix ViewProjMatrix;
    matrix ViewProjInvMatrix;
    float3 ViewPosition;
    float Dummy;
};

cbuffer ModelConstantBuffer : register(b1)
{
    matrix ModelMatrix;
    matrix ModelInvMatrix;
};


MeshObjectVertexOutput main(StaticVertexInput Input)
{
    MeshObjectVertexOutput Result;
       
    float d = distance(Input.f3WorldPos, ViewPosition);

    float MinTessDistance = 0.f;
    float MaxTessDistance = 1000.f;
    float MinTessFactor = 0.f;
    float MaxTessFactor = 3.f;
    float tess = 1.f - saturate(d / (MaxTessDistance - MinTessDistance));

    Result.fTessFactor = 2.f;
    Result.fLODLevel = 2.f;
    
    Result.f3ModelNormal = normalize(mul(float4(Input.f3WorldNormal, 0.f), ModelInvMatrix).xyz);
    Result.f3ModelTangent = normalize(mul(float4(Input.f3WorldTangent, 0.f), ModelMatrix).xyz);
    Result.f2TexCoord = Input.f2TexCoord;
    
    Result.f4ProjPos = mul(float4(Input.f3WorldPos, 1.f), ModelMatrix);
    Result.f4ProjPos = mul(Result.f4ProjPos, ViewProjMatrix);

    return Result;
}