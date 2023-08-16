#include "ShadersStructs.hlsli"


float4 main(PS_IN input) : SV_Target
{
    return input.pos;    
}