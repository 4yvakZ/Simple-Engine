#include "ShadersStructs.hlsli"

#if FORWARD
float4 main(PS_IN input) : SV_Target
{
    return input.pos;
}
#else
float4 main(PS_IN input) : SV_Target
{
    return input.pos;
}
#endif