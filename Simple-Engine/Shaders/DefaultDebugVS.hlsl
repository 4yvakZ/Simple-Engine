#include "ShadersStructs.hlsli"

DEBUG_PS_IN main(VS_IN input) : SV_POSITION
{
    DEBUG_PS_IN output = (DEBUG_PS_IN) 0;
    
    float4 worldPos = mul(float4(input.pos, 1.0f), world);
    output.pos = mul(worldPos, viewProjection);
    
    return output;
}