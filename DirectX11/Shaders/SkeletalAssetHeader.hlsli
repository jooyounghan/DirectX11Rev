struct BasicVertexInput
{
    float3 f3WorldPos : POSITION;
    float2 f2TexCoord : TEXCOORD;
    float3 f3WorldNormal : NORMAL;
    float3 f3WorldTangent : TANGENT;
    float3 f3WorldBitangent : BINORMAL;
    float4 f4BlendWeight : BLENDWEIGHT;
    uint4 f4BlendIndices : BLENDINDICES;
};

struct BasicVertexOutput
{
    float4 f4ProjPos : SV_Position;
    float2 f2TexCoord : TEXCOORD;
    float3 f3ModelNormal : NORMAL;
};

static const uint MATERIAL_IDX = 5;
static const uint AO_IDX        = 0;
static const uint SPECULAR_IDX  = 1;
static const uint DIFFUSE_IDX   = 2;
static const uint ROUGHNESS_IDX = 3;
static const uint METALIC_IDX   = 4;
static const uint NORMAL_IDX    = 5;
static const uint HEIGHT_IDX    = 6;
static const uint EMISSIVE_IDX  = 7;
