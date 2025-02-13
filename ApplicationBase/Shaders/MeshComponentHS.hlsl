#include "MeshComponentHeader.hlsli"


PatchTess CalcHSPatchConstants(
	InputPatch<MeshComponentVertexOutput, NUM_CONTROL_POINTS> patch,
	uint patchID : SV_PrimitiveID)
{
    PatchTess output;

    [unroll]
    for (int edge = 0; edge < 3; ++edge)
    {
        int nextEdge = (edge + 1) % 3;
        int previousEdge = (edge + 2) % 3;
        output.EdgeTessFactor[edge] = 0.5f * (patch[nextEdge].fTessFactor + patch[previousEdge].fTessFactor);
    }
    output.InsideTessFactor = (output.EdgeTessFactor[0] + output.EdgeTessFactor[1] + output.EdgeTessFactor[2]) / 3.f;
	
    return output;
}

[domain("tri")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("CalcHSPatchConstants")]
MeshComponentHullOutput main(
	InputPatch<MeshComponentVertexOutput, NUM_CONTROL_POINTS> patch,
	uint i : SV_OutputControlPointID,
	uint patchID : SV_PrimitiveID )
{
    MeshComponentHullOutput output;
    
    output.f4WorldPos = patch[i].f4WorldjPos;
    output.f2TexCoord = patch[i].f2TexCoord;
    output.f3WorldNormal = normalize(patch[i].f3WorldNormal);
    output.fLODLevel = patch[i].fLODLevel;

    return output;
}
