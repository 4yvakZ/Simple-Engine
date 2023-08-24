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

struct ALIGNED_QUAD_PS_IN
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD0;
};

struct DEBUG_PS_IN
{
    float4 pos : SV_Position;
};

struct GBuffer
{
    float4 worldPos : SV_Target0;
    float4 normal : SV_Target1;
    float4 albedo : SV_Target2;
    float4 metallicRoughnessAO : SV_Target3;
};

cbuffer FRAME_CONST_BUF : register(b0)
{
    Matrix view;
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

// shadowMap structs
struct GS_IN
{
    float4 pos : POSITION;
};

struct GS_OUT
{
    float4 pos : SV_POSITION;
    uint arrInd : SV_RenderTargetArrayIndex;
};

struct CascadeData
{
    Matrix viewProjection[4];
    float4 distances;
};

cbuffer CASCADE_CONST_BUF : register(b3)
{
    CascadeData cascade;
}

SamplerState Sampler : register(s0);
SamplerComparisonState ShadowSampler : register(s1);

Texture2D<float4> WorldPosTex : register(t0);
Texture2D<float4> AlbedoMap : register(t1);
Texture2D<float4> NormalMap : register(t2);
Texture2D<float4> MetallicRoughnessAOMap : register(t3);
Texture2D<float4> LightMap : register(t4);
Texture2DArray ShadowMap : register(t5);