#include "ShadersStructs.hlsli"

GS_IN main(VS_IN input)
{
    GS_IN output = (GS_IN) 0;
    
    output.pos = mul(float4(input.pos.xyz, 1.0f), world);

    return output;
}