#include "BloomP1.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
    float4 color = tex.Sample(smp, input.uv);
    color.xyz = color.xyz * float3(0.25f, 0.55f, 0.1f);
    float t = color.x + color.y + color.z;
    color.xyz = float3(color.x + t / 2.f, color.x + t / 2.f, color.x + t /2.f);
    
    return color;
}