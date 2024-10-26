#include "MeshObjectHeader.hlsli"

cbuffer MaterialAssetData : register(b0)
{
    bool IsAmbientOcculusionSet;
    bool IsSpecularSet;
    bool IsDiffuseSet;
    bool IsRoughnessSet;
    bool IsMetalicSet;
    bool IsNormalSet;
    bool IsHeightSet;
    bool IsEmissiveSet;
    float3 F0;
    bool Dummy1;
};

Texture2D HeightMap : register(t0);

SamplerState WrapSampler : register(s0);
SamplerState ClampSampler : register(s1);

[domain("tri")]
MeshObjectDomainOutput main(
	PatchTess input,
	float3 domain : SV_DomainLocation,
	const OutputPatch<MeshObjectHullOutput, NUM_CONTROL_POINTS> tri)
{
    MeshObjectDomainOutput Output;

    Output.f2TexCoord = tri[0].f2TexCoord * domain.x + tri[1].f2TexCoord * domain.y + tri[2].f2TexCoord * domain.z;
    Output.f3ModelNormal = tri[0].f3ModelNormal * domain.x + tri[1].f3ModelNormal * domain.y + tri[2].f3ModelNormal * domain.z;
    Output.f3ModelTangent = tri[0].f3ModelTangent * domain.x + tri[1].f3ModelTangent * domain.y + tri[2].f3ModelTangent * domain.z;
    Output.fLODLevel = tri[0].fLODLevel * domain.x + tri[1].fLODLevel * domain.y + tri[2].fLODLevel * domain.z;
	
    //float3 OutputProjPos = tri[0].f3ProjPos * domain.x + tri[1].f3ProjPos * domain.y + tri[2].f3ProjPos * domain.z;
    float4 OutputProjPos = tri[0].f4ProjPos * domain.x + tri[1].f4ProjPos * domain.y + tri[2].f4ProjPos * domain.z;

    if (IsHeightSet)
    {
        float Height = HeightMap.SampleLevel(WrapSampler, Output.f2TexCoord, Output.fLODLevel);
        Height = 2.f * Height - 1.f;
        OutputProjPos += float4(Height * Output.f3ModelNormal, 1.f);
    }
    Output.f4ProjPos = OutputProjPos;

	return Output;
}
