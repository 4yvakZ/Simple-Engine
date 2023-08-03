#pragma pack_matrix( row_major )
#define FORWARD true

struct VS_IN
{
    float3 pos : POSITION0;
    float4 color : COLOR0;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL0;
};

struct PS_IN
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD0;
    float4 worldPos : POSITION;
    float3 normal : NORMAL0;
};