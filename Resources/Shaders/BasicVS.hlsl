#include "Basic.hlsli"

VSOutput main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
	float3 lightDir = normalize(float3(1, -1, 1));
	float3 lightColor = float3(1.0f, 1.0f, 1.0f);

	VSOutput output;
	output.svpos = mul(mul(vpmat, mat), pos);

	const float3 eye = float3(0, 0, 20);
	const float luster = 4.0f;
	float3 eyeDir = normalize(eye - pos.xyz);

	float3 ambient = m_ambient;
	float3 diffuse = dot(-lightDir, normal) * m_diffuse;
	float3 reflect = normalize(lightDir + 2 * dot(-lightDir, normal)*normal);
	float3 specular = pow(saturate(dot(reflect, eyeDir)), luster) * m_specular;

	output.color.rgb = (ambient + diffuse + specular) * lightColor;
	output.color.a = m_alpha;
	output.uv = uv;

	return output;
}