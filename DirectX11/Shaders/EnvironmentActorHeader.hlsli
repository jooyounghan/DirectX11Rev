struct EnvironmentActorVertexInput
{
    float3 f3WorldPos : POSITION;
    float2 f2TexCoord : TEXCOORD;
    float3 f3WorldNormal : NORMAL;
};

struct EnvironmentActorVertexOutput
{
    float4 f4ProjPos : SV_Position;
    float2 f2TexCoord : TEXCOORD;
    float3 f3ModelNormal : NORMAL;
};