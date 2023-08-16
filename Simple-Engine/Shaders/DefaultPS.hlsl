#include "ShadersStructs.hlsli"


float4 main(PS_IN input) : SV_Target
{
    float3 color = AlbedoMap.Sample(Sampler, input.uv);
    
    float4 output = float4(pow(color, 0.45), 1);
    
    //output = NormalMap.Sample(Sampler, input.uv);
    
    return output;    
}