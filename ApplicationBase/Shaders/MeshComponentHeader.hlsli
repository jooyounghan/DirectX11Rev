struct SkeletalComponentVertexInput
{
    float3 f3WorldPos : POSITION;
    float2 f2TexCoord : TEXCOORD;
    float3 f3WorldNormal : NORMAL;
    float4 f4BlendWeight : BLENDWEIGHT;
    uint4 f4BlendIndices : BLENDINDICES;
};

struct StaticComponentVertexInput
{
    float3 f3WorldPos : POSITION;
    float2 f2TexCoord : TEXCOORD;
    float3 f3WorldNormal : NORMAL;
};

struct MeshComponentVertexOutput
{
    float4 f4WorldjPos : POSITION;
    float2 f2TexCoord : TEXCOORD;
    float3 f3WorldNormal : NORMAL;
    float fTessFactor : TESSFACTOR;
};

#define NUM_CONTROL_POINTS 3

struct PatchTess
{
    float EdgeTessFactor[3] : SV_TessFactor;
    float InsideTessFactor : SV_InsideTessFactor;
};

struct MeshComponentHullOutput
{
    float4 f4WorldPos : POSITION;
    float2 f2TexCoord : TEXCOORD;
    float3 f3WorldNormal : NORMAL;
};

struct MeshComponentDomainOutput
{
    float4 f4ProjPos : SV_Position;
    float4 f4ModelPos : POSITIONT;
    float2 f2TexCoord : TEXCOORD;
    float3 f3ModelNormal : NORMAL;
};

struct MeshComponentPixelOutput
{
    float4 f4Color;
    uint uiID;
};

struct DeferredMeshComponentPixelOutput
{
    float4 f4Position : SV_Target0;
    float4 f4Specular : SV_Target1;    
    float4 f4Diffuse : SV_Target2;
    float4 AO_Metallic_Roughness : SV_Target3;
    float4 f4Normal : SV_Target4;
    float4 Emissive : SV_Target5;
    uint uiID : SV_Target6;
};

void GetTB(float3 pos, float2 uv, float3 normal, out float3 tangent, out float3 bitangent)
{
    float3 dpdx = ddx(pos);
    float3 dpdy = ddy(pos);
    float2 duvdx = ddx(uv);
    float2 duvdy = ddy(uv);
    
    tangent = normalize(duvdx.y * dpdy - duvdy.y * dpdx);
    bitangent = normalize(-duvdx.x * dpdy + duvdy.x * dpdx);
    

    float3 x = cross(normal, tangent);
    tangent = normalize(cross(x, normal));

    x = cross(normal, bitangent);
    bitangent = normalize(cross(normal, x));
}