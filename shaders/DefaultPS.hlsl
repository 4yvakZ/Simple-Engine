#include "ShadersStructs.hlsli"

#ifdef FORWARD
float4 main(PS_IN input) : SV_Target
{
#ifdef PLAIN
    return input.color;
#else
    return input.pos;
#endif
    
}
#else
float4 main(PS_IN input) : SV_Target
{
    return input.pos;
}
#endif