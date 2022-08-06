#include "Basic.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
	float4 texcolor = float4(tex.Sample(smp, input.uv));

	float4 shadecolor;

	float3 eyeDir = normalize(cameraPos - input.worldpos.xyz);
	const float luster = 4.0f;

	//Directional Light
	{

		float3 dotLightNormal = dot(lightVec, input.normal);

		float3 ambient = m_ambient;
		float3 diffuse = dotLightNormal * m_diffuse;
		float3 reflect = normalize(-lightVec + 2 * dotLightNormal * input.normal);
		float3 specular = pow(saturate(dot(reflect, eyeDir)), luster) * m_specular;

		shadecolor.rgb = (ambient + diffuse + specular) * lightColor;
		shadecolor.a = m_alpha;
	}


	//Point Lights
	for (int i = 0; i < MAX_PLIGHTS; i++)
	{
		if (pointLights[i].isActive)
		{
			float3 lightVec = pointLights[i].lightPos - input.worldpos.xyz;
			float d = length(lightVec);
			lightVec = normalize(lightVec);

			float att = 1.0f / (pointLights[i].lightAtt.x + pointLights[i].lightAtt.y * d + pointLights[i].lightAtt.z * d * d);
			
			float3 dotLightNormal = dot(lightVec, input.normal);

			float3 diffuse = dotLightNormal * m_diffuse;

			float3 reflect = normalize(-lightVec + 2 * dotLightNormal * input.normal);

			float3 specular = specular = pow(saturate(dot(reflect, eyeDir)), luster) * m_specular;

			shadecolor.rgb += att * (diffuse + specular) * pointLights[i].lightColor;
		}
	}

	return shadecolor * texcolor * brightness;
}