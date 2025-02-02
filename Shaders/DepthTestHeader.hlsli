struct SkeletalMeshVertexInputForDepthTest
{
    float3 f3WorldPos : POSITION;
    float4 f4BlendWeight : BLENDWEIGHT;
    uint4 f4BlendIndices : BLENDINDICES;
};

struct StaticMeshVertexInputForDepthTest
{
    float3 f3WorldPos : POSITION;
};
