#include "BloomP1.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
    float4 color = tex.Sample(smp, input.uv);
    color.xyz = color.xyz * float3(0.3f, 0.6f, 0.1f);
    
    return color;
}