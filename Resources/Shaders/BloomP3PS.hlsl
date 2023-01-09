#include "BloomP3.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
    float4 color;
    
    color = weight[0].x * tex.Sample(smp, input.tex0.xy);
    color += weight[0].y * tex.Sample(smp, input.tex1.xy);
    color += weight[0].z * tex.Sample(smp, input.tex2.xy);
    color += weight[0].w * tex.Sample(smp, input.tex3.xy);
    color += weight[1].x * tex.Sample(smp, input.tex4.xy);
    color += weight[1].y * tex.Sample(smp, input.tex5.xy);
    color += weight[1].z * tex.Sample(smp, input.tex6.xy);
    color += weight[1].w * tex.Sample(smp, input.tex7.xy);
    
    color += weight[0].x * tex.Sample(smp, input.tex0.zw);
    color += weight[0].y * tex.Sample(smp, input.tex1.zw);
    color += weight[0].z * tex.Sample(smp, input.tex2.zw);
    color += weight[0].w * tex.Sample(smp, input.tex3.zw);
    color += weight[1].x * tex.Sample(smp, input.tex4.zw);
    color += weight[1].y * tex.Sample(smp, input.tex5.zw);
    color += weight[1].z * tex.Sample(smp, input.tex6.zw);
    color += weight[1].w * tex.Sample(smp, input.tex7.zw);
    
    return float4(color.xyz, 1.0f);
}