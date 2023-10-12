#include "BloomP2.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
    float4 color;
    
    color = weight[0].x * tex.Sample(smp,  clamp(input.tex0.xy, 0.f, 0.99999f));
    color += weight[0].y * tex.Sample(smp, clamp(input.tex1.xy, 0.f, 0.99999f));
    color += weight[0].z * tex.Sample(smp, clamp(input.tex2.xy, 0.f, 0.99999f));
    color += weight[0].w * tex.Sample(smp, clamp(input.tex3.xy, 0.f, 0.99999f));
    color += weight[1].x * tex.Sample(smp, clamp(input.tex4.xy, 0.f, 0.99999f));
    color += weight[1].y * tex.Sample(smp, clamp(input.tex5.xy, 0.f, 0.99999f));
    color += weight[1].z * tex.Sample(smp, clamp(input.tex6.xy, 0.f, 0.99999f));
    color += weight[1].w * tex.Sample(smp, clamp(input.tex7.xy, 0.f, 0.99999f));
    
    color += weight[0].x * tex.Sample(smp, clamp(input.tex0.zw, 0.f, 0.99999f));
    color += weight[0].y * tex.Sample(smp, clamp(input.tex1.zw, 0.f, 0.99999f));
    color += weight[0].z * tex.Sample(smp, clamp(input.tex2.zw, 0.f, 0.99999f));
    color += weight[0].w * tex.Sample(smp, clamp(input.tex3.zw, 0.f, 0.99999f));
    color += weight[1].x * tex.Sample(smp, clamp(input.tex4.zw, 0.f, 0.99999f));
    color += weight[1].y * tex.Sample(smp, clamp(input.tex5.zw, 0.f, 0.99999f));
    color += weight[1].z * tex.Sample(smp, clamp(input.tex6.zw, 0.f, 0.99999f));
    color += weight[1].w * tex.Sample(smp, clamp(input.tex7.zw, 0.f, 0.99999f));
    
    return float4(color.xyz, 1.0f);
}