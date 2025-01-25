struct SkeletalComponentVertexInput
{
    float3 f3WorldPos : POSITION;
    float2 f2TexCoord : TEXCOORD;
    float3 f3WorldNormal : NORMAL;
    float3 f3WorldTangent : TANGENT;
    float4 f4BlendWeight : BLENDWEIGHT;
    uint4 f4BlendIndices : BLENDINDICES;
};

struct StaticComponentVertexInput
{
    float3 f3WorldPos : POSITION;
    float2 f2TexCoord : TEXCOORD;
    float3 f3WorldNormal : NORMAL;
    float3 f3WorldTangent : TANGENT;
};

struct MeshComponentVertexOutput
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

struct MeshComponentHullOutput
{
    float4 f4WorldPos : POSITION;
    float2 f2TexCoord : TEXCOORD;
    float3 f3WorldNormal : NORMAL;
    float3 f3ModelTangent : TANGENT;
    float fLODLevel : LOD_LEVEL;
};

struct MeshComponentDomainOutput
{
    float4 f4ProjPos : SV_Position;
    float4 f4ModelPos : POSITIONT;
    float2 f2TexCoord : TEXCOORD;
    float3 f3ModelNormal : NORMAL;
    float3 f3ModelTangent : TANGENT;
    float fLODLevel : LOD_LEVEL;
};

struct MeshComponentPixelOutput
{
    float4 f4Color;
    uint4 f4ID;
};

struct DefferedMeshComponentPixelOutput
{
    float4 f4Position : SV_Target0;
    float4 f4Specular : SV_Target1;    
    float4 f4Diffuse : SV_Target2;
    float4 AO_Metallic_Roughness : SV_Target3;
    float4 f4Normal : SV_Target4;
    float4 Emissive : SV_Target5;
    float4 FresnelReflectance : SV_Target6;
    uint4 f4ID : SV_Target7;
};