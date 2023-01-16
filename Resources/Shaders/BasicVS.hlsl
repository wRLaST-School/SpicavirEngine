#include "Basic.hlsli"

VSOutput main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
	//float3 lightDir = normalize(float3(1, -1, 1));
	//float3 lightColor = float3(1.0f, 1.0f, 1.0f);

	float4 wnormal = normalize(mul(mat, float4(normal, 0)));
    //float4 wnormal = normal;
	float4 wpos = mul(mat, pos);

	VSOutput output;
	output.svpos = mul(mul(vpmat, mat), pos);
	output.worldpos = wpos;
	output.normal = wnormal.xyz;
	output.uv = uv;

	return output;
}