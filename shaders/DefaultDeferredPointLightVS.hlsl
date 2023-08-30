#include "ShadersStructs.hlsli"

ALIGNED_QUAD_PS_IN main(VS_IN input)
{
    ALIGNED_QUAD_PS_IN output = (ALIGNED_QUAD_PS_IN) 0;
    
    float4 worldPos = mul(float4(input.pos, 1.0f), world);
    output.pos = mul(worldPos, viewProjection);
    output.uv = output.pos.xy;
    float4 centerPos = mul(float4(0, 0, 0, 1), viewProjection);
    output.uv = (output.pos.xy / output.pos.w * float2(1, -1) + float2(1, 1)) * 0.5;
    
    return output;
}