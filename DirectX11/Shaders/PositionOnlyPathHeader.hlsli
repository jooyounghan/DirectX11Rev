struct PositionOnlyVertexInput
{
    float3 f3WorldPos : POSITION;
};

struct PositionOnlyVertexOutPut
{
    float4 f4ProjPos : SV_Position;
};

struct PickinIDPixelOutput
{
    float4 f4ProjPos : SV_Position;
};
