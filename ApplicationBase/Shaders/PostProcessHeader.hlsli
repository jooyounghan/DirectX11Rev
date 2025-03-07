struct PostProcessVertexInput
{
    float3 f3WorldPos : POSITION;
    float2 f2TexCoord : TEXCOORD;
};

struct PostProcessVertexOutput
{
    float4 f4WorldjPos : SV_Position;
    float2 f2TexCoord : TEXCOORD;
};

static const float2 poisson16[16] =
{
    float2(-0.94201624, -0.39906216), float2(0.94558609, -0.76890725),
    float2(-0.094184101, -0.92938870), float2(0.34495938, 0.29387760),
    float2(-0.91588581, 0.45771432), float2(-0.81544232, -0.87912464),
    float2(-0.38277543, 0.27676845), float2(0.97484398, 0.75648379),
    float2(0.44323325, -0.97511554), float2(0.53742981, -0.47373420),
    float2(-0.26496911, -0.41893023), float2(0.79197514, 0.19090188),
    float2(-0.24188840, 0.99706507), float2(-0.81409955, 0.91437590),
    float2(0.19984126, 0.78641367), float2(0.14383161, -0.14100790)
};

float3 GetGaussianBluredSample16(
    Texture2D sampledTexture, 
    SamplerState samplerState, 
    float2 uv, float2 dxdy, float radius
)
{
    float3 color = float3(0.f, 0.f, 0.f);
    for (int i = 0; i < 16; ++i)
    {
        float2 offset = dxdy * poisson16[i];
        color += sampledTexture.Sample(samplerState, uv + offset * radius).rgb;
    }
    color /= 16.f;
    return color;
}

float ComputeCoC(float distance, float focalDistance, float aperture)
{
    // 실험적 결과 ========================================================
    float focalPlane = (focalDistance * distance) / (distance - focalDistance);
    float coc = focalDistance / (aperture * abs(focalDistance - focalPlane));
    return saturate(coc);
    // =======================================================================    


    // UnrealEngine/Engine/Shaders/Private/ CircleDOFCommon.ush Style ========
    //float cocRadius = (distance - focalDistance) * focalDistance / distance;
    //float depthBlurRadius = 1.f - exp2(-distance / aperture);
    
    //float returnCoc = max(abs(cocRadius), depthBlurRadius) * sign(cocRadius);
    //return clamp(returnCoc, -1, 1);
    // =======================================================================
}