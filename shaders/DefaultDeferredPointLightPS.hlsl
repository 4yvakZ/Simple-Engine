#include "ShadersStructs.hlsli"

static const float PI = 3.14159265359;
  
float DistributionGGX(float3 N, float3 H, float roughness);
float GeometrySchlickGGX(float NdotV, float roughness);
float GeometrySmith(float3 N, float3 V, float3 L, float roughness);
float3 fresnelSchlick(float cosTheta, float3 F0);
int getCascadeLayer(float depthValue);

float4 main(ALIGNED_QUAD_PS_IN input) : SV_Target
{
    //return float4(input.uv, 0, 1);
    uint width, height;
    WorldPosTex.GetDimensions(width, height);
    int2 pixelCoord = int2(input.uv.x * width, input.uv.y * height);
    
    float4 worldPos = WorldPosTex.Load(int3(pixelCoord, 0));
    
    float3 albedo = AlbedoMap.Load(int3(pixelCoord, 0));
    
    float3 normal = NormalMap.Load(int3(pixelCoord, 0));
    
    float3 metallicRoughnessAO = MetallicRoughnessAOMap.Load(int3(pixelCoord, 0));
    float metallic = metallicRoughnessAO.r;
    float roughness = metallicRoughnessAO.g;
       
    float3 F0 = float3(0.04, 0.04, 0.04);
    F0 = lerp(F0, albedo, metallic);
    
    float3 lightDir = normalize(light.position - worldPos);
    float distance = length(light.position - worldPos);
    float attenuation = 1.0 / (distance * distance);
    float3 intensity = light.intensity.xyz * attenuation;
    
    float3 view = normalize(cameraPos - worldPos);
    float3 halfVector = normalize(view + lightDir);
    
    // cook-torrance brdf
    float NDF = DistributionGGX(normal, halfVector, roughness);
    float G = GeometrySmith(normal, view, lightDir, roughness);
    float3 F = fresnelSchlick(max(dot(halfVector, view), 0.0), F0);
        
    float3 kS = F;
    float3 kD = 1.0 - kS;
    kD *= 1.0 - metallic;
        
    float3 numerator = NDF * G * F;
    float denominator = 4.0 * max(dot(normal, view), 0.0)
    * max(dot(normal, lightDir), 0.0) + 0.0001;
    float3 specular = numerator / denominator;
            
    // add to outgoing radiance Lo
    float NdotL = max(dot(normal, lightDir), 0.0);
    float3 Lo = (kD * albedo / PI + specular) * intensity * NdotL;
    
    return float4(Lo, 1.0f);
}


float3 fresnelSchlick(float cosTheta, float3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

float DistributionGGX(float3 N, float3 H, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;
	
    float num = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
	
    return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

    float num = NdotV;
    float denom = NdotV * (1.0 - k) + k;
	
    return num / denom;
}

float GeometrySmith(float3 N, float3 V, float3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);
	
    return ggx1 * ggx2;
}

int getCascadeLayer(float depthValue)
{
    int layer = 3;
    for (int i = 0; i < 4; i++)
    {
        if (depthValue < cascade.distances[i])
        {
            layer = i;
            break;
        }
    }
    return layer;
}