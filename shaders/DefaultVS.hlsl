#include "ShadersStructs.hlsli"

PS_IN main(VS_IN input)
{
    PS_IN output = (PS_IN) 0;
    
    output.worldPos = mul(float4(input.pos, 1.0f), world);
    output.pos = mul(output.worldPos, viewProjection);
    output.uv = input.uv;
    
    float3 normal = normalize(mul(float4(input.normal, 0.0f), transpInvWorld));
    float3 tangent = normalize(mul(float4(input.tangent, 0.0f), transpInvWorld));
    //tangent = normalize(tangent - dot(tangent, normal) * normal);
    
    float3 bitangent = cross(normal, tangent);
    
    
    output.TBN = float3x3(tangent, bitangent, normal);
    
    output.normal = mul(float4(input.normal, 0.0f), transpInvWorld);
    
    return output;
}