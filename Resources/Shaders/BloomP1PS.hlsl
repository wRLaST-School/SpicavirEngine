#include "Sprite.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
    float4 color = tex.Sample(smp, input.uv);
    
    float4 t = dot(color.xyz, float3(0.2125f, 0.7154f, 0.0721f));
    clip(t - 1.0f);
    
    return color;
}