struct BoundingObjectVertexInput
{
    float3 f3WorldPos : POSITION;
};

struct BoundingObjectVertexOutPut
{
    float4 f4ProjPos : SV_Position;
};

struct BoundingObjectPixelOutPut
{
    float4 f4Color : SV_TARGET0;
    float4 f4ID : SV_TARGET1;
};

struct DefferedBoundingObjectPixelOutput
{
    float4 f4BaseColor : SV_Target0;
    float4 f4Normal : SV_Target1;
    float4 AO_Metallic_Roughness : SV_Target2;
    float4 Emissive : SV_Target3;
    float4 f4ID : SV_Target4;
};