#include "Particle.hlsli"

VSOutput main(float4 pos : POSITION, float scale : SCALE, float4 color : COLOR)
{
	//float3 lightDir = normalize(float3(1, -1, 1));
	//float3 lightColor = float3(1.0f, 1.0f, 1.0f);

    //float4 wnormal = normalize(mul(mat, float4(normal, 0)));
    float4 wpos = pos;

    VSOutput output;
    output.svpos = pos;
    output.worldpos = wpos;
    output.color = color;
    output.scale = scale;
    //output.normal = wnormal.xyz;
    //output.uv = uv;

    return output;
}