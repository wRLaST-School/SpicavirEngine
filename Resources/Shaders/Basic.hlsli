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

//change MAX_LIGHTS in PointLight.h aswell
static const int MAX_PLIGHTS = 16;
struct PointLight
{
	float3 lightPos;
	float3 lightColor;
	float3 lightAtt;
	uint isActive;
};

cbuffer cbuff4 : register(b4)
{
	float3 lightVec;
	float3 lightColor;

	PointLight pointLights[MAX_PLIGHTS];
}

static const int MAX_BONES = 128;

cbuffer cbuff5 : register(b5)
{
    matrix bMatrix[MAX_BONES];
}

struct VSOutput
{
	float4 svpos : SV_POSITION;
	float4 worldpos : POSITION;
	float3 normal : NORMAL;
	float2 uv :TEXCOORD;
};

struct GSOutput
{
    float4 svpos : SV_POSITION;
    float4 worldpos : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
};