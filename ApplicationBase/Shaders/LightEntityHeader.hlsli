struct LightEntity
{
    float lightPower;
    float fallOffStart;
    float fallOffEnd;
    float spotPower;
};

struct LightViewEntity
{
    matrix viewProjMatrix;
    matrix viewProjInvMatrix;
    float3 viewPosition;
    float dummy;
};