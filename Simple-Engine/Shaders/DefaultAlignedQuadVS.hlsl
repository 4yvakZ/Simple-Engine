#include "ShadersStructs.hlsli"


ALIGNED_QUAD_PS_IN main(uint id : SV_VertexID)
{
    ALIGNED_QUAD_PS_IN output = (ALIGNED_QUAD_PS_IN) 0;
    
    output.uv = float2(id & 1, id >> 1);
    output.pos = float4(output.uv * float2(2, -2) + float2(-1, 1), 0, 1);
    
    return output;
}