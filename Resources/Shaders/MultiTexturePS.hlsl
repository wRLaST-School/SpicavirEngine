#include "MultiTexture.hlsli"

Texture2D<float4> tex0 : register(t0);
Texture2D<float4> tex1 : register(t1);
SamplerState smp : register(s0);

struct PSOutput
{
    float4 target0 : SV_TARGET;
};

PSOutput main(VSOutput input) : SV_TARGET
{
    float4 texcolor;
	
    if (fmod(input.uv.y, 0.1f) < 0.05f)
    {
        texcolor = float4(tex0.Sample(smp, input.uv));
    }
	else
    {
        texcolor = float4(tex1.Sample(smp, input.uv));
    }
    
    PSOutput output;
    output.target0 = texcolor;
	
	return output;
}