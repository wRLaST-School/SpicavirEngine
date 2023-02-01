cbuffer cbuff0 : register(b0)
{
	float3 m_ambient : packoffset(c0);
	float3 m_diffuse : packoffset(c1);
	float3 m_specular : packoffset(c2);
	float m_alpha : packoffset(c2.w);
}

cbuffer cbuff1 : register(b1)
{
	matrix mat;
}

cbuffer cbuff2 : register(b2)
{
	matrix vpmat;
    matrix billboardMat;
	float3 cameraPos;
}

cbuffer cbuff3 : register(b3)
{
	float4 brightness;
}

struct VSOutput
{
	float4 svpos : SV_POSITION;
	float4 worldpos : POSITION;
	float3 normal : NORMAL;
	float2 uv :TEXCOORD;
};