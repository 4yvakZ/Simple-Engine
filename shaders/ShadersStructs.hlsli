#pragma pack_matrix( row_major )

struct VS_IN
{
    float3 pos : POSITION0;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL0;
    float3 tangent : TANGENT0;
};

 
struct PS_IN
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD0;
    float4 worldPos : POSITION0;
    float3 normal : NORMAL0;
    float3x3 TBN : TANGENT_TRANSFORM;
};

cbuffer FRAME_CONST_BUF : register(b0)
{
    Matrix viewProjection;
    float4 cameraPos;
}

cbuffer OBJECT_CONST_BUF : register(b1)
{
    Matrix world;
    Matrix transpInvWorld;
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

Texture2D<float4> AlbedoMap : register(t0);
Texture2D<float4> NormalMap : register(t1);
Texture2D<float> MetallicMap : register(t2);
Texture2D<float> RoughnessMap : register(t3);
Texture2D<float> AOMap : register(t4);