#include "ShadersStructs.hlsli"

PS_IN main(VS_IN input)
{
    PS_IN output = (PS_IN) 0;
    
    output.worldPos = float4(input.pos, 1.0f);
    
    output.pos = float4(input.pos, 1.0f);
    
    output.uv = input.uv;
    output.normal = input.normal;

    return output;
}