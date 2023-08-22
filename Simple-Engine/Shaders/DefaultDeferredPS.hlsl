#include "ShadersStructs.hlsli"


GBuffer main(PS_IN input)
{
    GBuffer output = (GBuffer) 0;
    
    output.worldPos = input.worldPos;
    output.albedo = pow(AlbedoMap.Sample(Sampler, input.uv), 2.2);
    float3 normal = NormalMap.Sample(Sampler, input.uv);
    normal = normal * 2.0 - 1.0;
    normal = normalize(mul(normal, input.TBN));
    output.normal = float4(normal, 0.0);
    output.metallicRoughnessAO = MetallicRoughnessAOMap.Sample(Sampler, input.uv);
    
    return output;
}