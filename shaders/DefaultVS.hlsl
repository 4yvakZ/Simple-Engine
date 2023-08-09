#include "ShadersStructs.hlsli"

PS_IN main(VS_IN input)
{
    PS_IN output = (PS_IN) 0;
    
    output.worldPos = float4(input.pos, 1.0f);
    
    output.pos = mul(float4(input.pos, 1.0f), viewProjection);
    
#ifdef PLAIN
    output.color = input.color;
#else
    output.uv = input.uv;
#endif
    output.normal = input.normal;

    return output;
}