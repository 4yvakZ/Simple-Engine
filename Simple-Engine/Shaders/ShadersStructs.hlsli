#pragma pack_matrix( row_major )

struct VS_IN
{
    float3 pos : POSITION0;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL0;
};

 
struct PS_IN
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD0;
    float4 worldPos : POSITION0;
    float4 normal : NORMAL0;
};

cbuffer FRAME_CONST_BUF : register(b0)
{
    Matrix viewProjection;
    float4 cameraPos;
}

cbuffer OBJECT_CONST_BUF : register(b1)
{
    Matrix world;
    Matrix invWorld;
}