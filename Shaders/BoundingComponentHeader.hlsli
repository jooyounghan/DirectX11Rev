struct BoundingComponentVertexInput
{
    float3 f3WorldPos : POSITION;
};

struct BoundingComponentVertexOutPut
{
    float4 f4ProjPos : SV_Position;
};

struct BoundingComponentPixelOutPut
{
    float4 f4Color : SV_TARGET0;
    float4 f4ID : SV_TARGET1;
};