#include "ShadersStructs.hlsli"

float4 main(ALIGNED_QUAD_PS_IN input) : SV_TARGET
{
    uint width, height;
    AlbedoMap.GetDimensions(width, height);
    int2 pixelCoord = int2(input.uv.x * width, input.uv.y * height);
    
    float4 albedo = AlbedoMap.Load(int3(pixelCoord, 0));
    float3 lighting = LightMap.Load(int3(pixelCoord, 0));
    float ao = MetallicRoughnessAOMap.Load(int3(pixelCoord, 0)).b;
    float3 ambient = 0.03 * albedo * ao;
    
    return float4(pow(lighting + ambient, 0.45), 1.0f);
}