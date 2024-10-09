#include "SkeletalAssetHeader.hlsli"

TextureCube SpecularTexture : register(t0);
TextureCube DiffuseTexture : register(t1);
Texture2D BRDFTexture : register(t2);

Texture2D MaterialTexture[8] : register(t3);

SamplerState WrapSampler : register(s0);
SamplerState ClampSampler : register(s1);

float4 main(BasicVertexOutput Input) : SV_TARGET
{
    uint offset = Input.f2TexCoord.x / 1;

    return MaterialTexture[DIFFUSE_IDX].Sample(WrapSampler, float2(Input.f2TexCoord.x - offset, Input.f2TexCoord.y));
    
    //return SpecularTexture.Sample(WrapSampler, Input.f3ModelNormal);
}