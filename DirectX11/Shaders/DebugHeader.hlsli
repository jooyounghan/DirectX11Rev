struct DebugVertexInput
{
    float3 f3WorldPos : POSITION;
    float2 f2TexCoord : TEXCOORD;
};

struct DebugVertexOutput
{
    float4 f4ProjPos : SV_Position;
    float2 f2TexCoord : TEXCOORD;
};