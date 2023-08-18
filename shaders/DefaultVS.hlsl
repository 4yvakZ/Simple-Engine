#include "ShadersStructs.hlsli"

PS_IN main(VS_IN input)
{
    PS_IN output = (PS_IN) 0;
    
    output.worldPos = mul(float4(input.pos, 1.0f), world);
    output.pos = mul(output.worldPos, viewProjection);
    output.uv = input.uv;
    output.normal = mul(float4(input.normal, 0.0f), transpInvWorld);
    
    return output;
}