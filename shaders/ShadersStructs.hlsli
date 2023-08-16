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

struct LightData
{
    float4 direction;
    float4 intensity;
};

cbuffer LIGHT_CONST_BUF : register(b2)
{
    LightData light;
}

SamplerState Sampler : register(s0);

Texture2D AlbedoMap : register(t0);
Texture2D NormalMap : register(t1);
Texture2D MetalicMap : register(t2);
Texture2D RoughnessMap : register(t3);
Texture2D AOMap : register(t4);