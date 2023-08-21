#include "ShadersStructs.hlsli"

static const float PI = 3.14159265359;
  
float DistributionGGX(float3 N, float3 H, float roughness);
float GeometrySchlickGGX(float NdotV, float roughness);
float GeometrySmith(float3 N, float3 V, float3 L, float roughness);
float3 fresnelSchlick(float cosTheta, float3 F0);

float4 main(PS_IN input) : SV_Target
{
    float3 albedo = pow(AlbedoMap.Sample(Sampler, input.uv).xyz, float3(2.2, 2.2, 2.2));

    float3 normal = NormalMap.Sample(Sampler, input.uv);
    normal = normal * 2.0 - 1.0;
    normal = normalize(mul(normal, input.TBN));
    //return float4(normal, 1.0);
    
    float metallic = MetallicMap.Sample(Sampler, input.uv);
    float roughness = RoughnessMap.Sample(Sampler, input.uv);
    float ao = AOMap.Sample(Sampler, input.uv);
       
    float3 F0 = float3(0.04, 0.04, 0.04);
    F0 = lerp(F0, albedo, metallic);
    
    float3 lightDir = -light.direction.xyz;
    float3 view = normalize(cameraPos - input.worldPos);
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
    float3 Lo = (kD * albedo / PI + specular) * light.intensity.xyz * NdotL;
    
    float3 output = float4(pow(albedo, 0.45), 1);
    
    float3 ambient = 0.03 * albedo * ao;
   
    float3 color = Lo + ambient;
	
    color = color / (color + 1.0);
    color = pow(color, 0.44);
    
    return float4(color, 1.0f);
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