#pragma pack_matrix( row_major )

#define FORWARD true
#define PLAIN true
struct VS_IN
{
    float3 pos : POSITION0;
    float4 color : COLOR0;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL0;
};

#ifdef PLAIN
struct PS_IN
{
    float4 pos : SV_Position;
    float4 color : COLOR0;
    float4 worldPos : POSITION0;
    float3 normal : NORMAL0;
};
#else 
struct PS_IN
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD0;
    float4 worldPos : POSITION0;
    float3 normal : NORMAL0;
};
#endif