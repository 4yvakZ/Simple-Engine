#include "ShadersStructs.hlsli"


float4 main(LIGHT_PS_IN input) : SV_Target
{
    uint width, height;
    WorldPosTex.GetDimensions(width, height);
    uint2 pixelCoord = uint2(input.uv.x * width, input.uv.y * height);
    
    return WorldPosTex.Load(uint3(pixelCoord, 0));
}