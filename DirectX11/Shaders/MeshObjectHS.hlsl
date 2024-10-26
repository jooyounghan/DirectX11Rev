#include "MeshObjectHeader.hlsli"


PatchTess CalcHSPatchConstants(
	InputPatch<MeshObjectVertexOutput, NUM_CONTROL_POINTS> Patch,
	uint PatchID : SV_PrimitiveID)
{
    PatchTess Output;

    Output.EdgeTessFactor[0] = 0.5f * (Patch[1].fTessFactor + Patch[2].fTessFactor);
    Output.EdgeTessFactor[1] = 0.5f * (Patch[0].fTessFactor + Patch[2].fTessFactor);
    Output.EdgeTessFactor[2] = 0.5f * (Patch[0].fTessFactor + Patch[1].fTessFactor);

    Output.InsideTessFactor = (Output.EdgeTessFactor[0] + Output.EdgeTessFactor[1] + Output.EdgeTessFactor[2]) / 3.f;
	
	return Output;
}

[domain("tri")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("CalcHSPatchConstants")]
MeshObjectHullOutput main(
	InputPatch<MeshObjectVertexOutput, NUM_CONTROL_POINTS> Patch,
	uint i : SV_OutputControlPointID,
	uint PatchID : SV_PrimitiveID )
{
    MeshObjectHullOutput Output;
    
    Output.f4ProjPos = Patch[i].f4ProjPos;
    Output.f2TexCoord = Patch[i].f2TexCoord;
    Output.f3ModelNormal = Patch[i].f3ModelNormal;
    Output.f3ModelTangent = Patch[i].f3ModelTangent;
    Output.fLODLevel = Patch[i].fLODLevel;

	return Output;
}
