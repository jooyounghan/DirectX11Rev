struct SkeletalVertexInput
{
    float3 f3WorldPos : POSITION;
    float2 f2TexCoord : TEXCOORD;
    float3 f3WorldNormal : NORMAL;
    float3 f3WorldTangent : TANGENT;
    float4 f4BlendWeight : BLENDWEIGHT;
    uint4 f4BlendIndices : BLENDINDICES;
};

struct StaticVertexInput
{
    float3 f3WorldPos : POSITION;
    float2 f2TexCoord : TEXCOORD;
    float3 f3WorldNormal : NORMAL;
    float3 f3WorldTangent : TANGENT;
};

struct MeshObjectVertexOutput
{
    float4 f4WorldjPos : POSITION;
    float2 f2TexCoord : TEXCOORD;
    float3 f3WorldNormal : NORMAL;
    float3 f3ModelTangent : TANGENT;
    float fTessFactor : TESSFACTOR;
    float fLODLevel : LOD_LEVEL;
};

#define NUM_CONTROL_POINTS 3

struct PatchTess
{
    float EdgeTessFactor[3] : SV_TessFactor;
    float InsideTessFactor : SV_InsideTessFactor;
};

struct MeshObjectHullOutput
{
    float4 f4WorldPos : POSITION;
    float2 f2TexCoord : TEXCOORD;
    float3 f3WorldNormal : NORMAL;
    float3 f3ModelTangent : TANGENT;
    float fLODLevel : LOD_LEVEL;
};

struct MeshObjectDomainOutput
{
    float4 f4ProjPos : SV_Position;
    float4 f3ModelPos : POSITIONT;
    float2 f2TexCoord : TEXCOORD;
    float3 f3ModelNormal : NORMAL;
    float3 f3ModelTangent : TANGENT;
    float fLODLevel : LOD_LEVEL;
};

struct MeshObjectPixelOutput
{
    float4 f4Color;
    float4 f4ID;
};

struct DefferedMeshObjectPixelOutput
{
    float4 f4BaseColor : SV_Target0;
    float4 f4Normal : SV_Target1;
    float4 AO_Metallic_Roughness : SV_Target2;
    float4 Emissive : SV_Target3;
    float4 f4ID : SV_Target4;
};