#include "MeshComponentHeader.hlsli"


PatchTess CalcHSPatchConstants(
	InputPatch<MeshComponentVertexOutput, NUM_CONTROL_POINTS> Patch,
	uint PatchID : SV_PrimitiveID)
{
    PatchTess Output;

    [unroll]
    for (int edge = 0; edge < 3; ++edge)
    {
        int nextEdge = (edge + 1) % 3;
        int previousEdge = (edge + 2) % 3;
        Output.EdgeTessFactor[edge] = 0.5f * (Patch[nextEdge].fTessFactor + Patch[previousEdge].fTessFactor);
    }
    Output.InsideTessFactor = (Output.EdgeTessFactor[0] + Output.EdgeTessFactor[1] + Output.EdgeTessFactor[2]) / 3.f;
	
	return Output;
}

[domain("tri")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("CalcHSPatchConstants")]
MeshComponentHullOutput main(
	InputPatch<MeshComponentVertexOutput, NUM_CONTROL_POINTS> Patch,
	uint i : SV_OutputControlPointID,
	uint PatchID : SV_PrimitiveID )
{
    MeshComponentHullOutput Output;
    
    Output.f4WorldPos = Patch[i].f4WorldjPos;
    Output.f2TexCoord = Patch[i].f2TexCoord;
    Output.f3WorldNormal = normalize(Patch[i].f3WorldNormal);
    Output.fLODLevel = Patch[i].fLODLevel;

	return Output;
}
