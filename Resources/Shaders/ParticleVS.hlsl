#include "Particle.hlsli"

VSOutput main(float4 pos : POSITION, float scale : SCALE, float4 color : COLOR)
{
    float4 wpos = pos;

    VSOutput output;
    output.svpos = pos;
    output.worldpos = wpos;
    output.color = color;
    output.scale = scale;

    return output;
}