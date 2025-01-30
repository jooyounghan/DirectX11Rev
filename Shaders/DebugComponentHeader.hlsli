struct DebugComponentVertexInput
{
    float3 f3WorldPos : POSITION;
};

struct DebugComponentVertexOutPut
{
    float4 f4ProjPos : SV_Position;
};

struct DebugComponentPixelOutPut
{
    float4 f4Color : SV_TARGET0;
    uint uiID : SV_TARGET1;
};