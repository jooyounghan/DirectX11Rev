struct SkeletalVertexInput
{
    float3 f3WorldPos : POSITION;
    float2 f2TexCoord : TEXCOORD;
    float3 f3WorldNormal : NORMAL;
    float3 f3WorldTangent : TANGENT;
    float3 f3WorldBitangent : BINORMAL;
    float4 f4BlendWeight : BLENDWEIGHT;
    uint4 f4BlendIndices : BLENDINDICES;
};

struct StaticVertexInput
{
    float3 f3WorldPos : POSITION;
    float2 f2TexCoord : TEXCOORD;
    float3 f3WorldNormal : NORMAL;
    float3 f3WorldTangent : TANGENT;
    float3 f3WorldBitangent : BINORMAL;
};

struct MeshObjectVertexOutput
{
    float4 f4ProjPos : SV_Position;
    float2 f2TexCoord : TEXCOORD;
    float3 f3ModelNormal : NORMAL;
};
