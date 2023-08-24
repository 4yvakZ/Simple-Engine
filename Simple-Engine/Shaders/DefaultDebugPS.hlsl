#include "ShadersStructs.hlsli"

float4 main(DEBUG_PS_IN input) : SV_TARGET
{
    return AlbedoMap.Load(int3(0, 0, 0));
}