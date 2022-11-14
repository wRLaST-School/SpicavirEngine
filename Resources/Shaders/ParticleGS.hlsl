#include "Particle.hlsli"

static const uint vnum = 4;
static const float4 offset_array[vnum] =
{
    float4(-0.5f, -0.5f, 0, 0), //BL
    float4(-0.5f, +0.5f, 0, 0), //TL
    float4(+0.5f, -0.5f, 0, 0), //BR
    float4(+0.5f, +0.5f, 0, 0) //TR
};

static const float2 uv_array[vnum] =
{
    float2(0, 1),
    float2(0, 0),
    float2(1, 1),
    float2(1, 0)
};

[maxvertexcount(vnum)]
void main(
	point VSOutput input[1] : SV_POSITION,
	inout TriangleStream<GSOutput> output
)
{
    GSOutput element;
    for (uint i = 0; i < vnum; i++)
    {
        element.svpos = input[0].svpos + offset_array[i];
        element.svpos = mul(vpmat, element.svpos);
        element.uv = uv_array[i];
        output.Append(element);
    }
}