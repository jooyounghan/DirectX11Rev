struct GBufferResolveVertexInput
{
    float3 f3WorldPos : POSITION;
    float2 f2TexCoord : TEXCOORD;
};

struct GBufferResolveVertexOutput
{
    float4 f4WorldjPos : SV_Position;
    float2 f2TexCoord : TEXCOORD;
};
