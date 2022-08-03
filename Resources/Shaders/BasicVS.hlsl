#include "Basic.hlsli"

VSOutput main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
	//float3 lightDir = normalize(float3(1, -1, 1));
	//float3 lightColor = float3(1.0f, 1.0f, 1.0f);

	float4 wnormal = normalize(mul(mat, float4(normal, 0)));
	float4 wpos = mul(mat, pos);

	VSOutput output;
	output.svpos = mul(mul(vpmat, mat), pos);

	const float luster = 4.0f;
	float3 eyeDir = normalize(cameraPos - wpos.xyz);

	float3 ambient = m_ambient;
	float3 diffuse = dot(lightVec, wnormal.xyz) * m_diffuse;
	float3 reflect = normalize(-lightVec + 2 * dot(-lightVec, wnormal.xyz)*wnormal.xyz);
	float3 specular = pow(saturate(dot(reflect, eyeDir)), luster) * m_specular;

	output.color.rgb = (ambient + diffuse + specular) * lightColor;
	output.color.a = m_alpha;
	output.uv = uv;

	return output;
}