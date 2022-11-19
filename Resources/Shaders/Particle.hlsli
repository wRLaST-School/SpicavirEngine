cbuffer cbuff0 : register(b0)
{
    float3 m_ambient : packoffset(c0);
    float3 m_diffuse : packoffset(c1);
    float3 m_specular : packoffset(c2);
    float m_alpha : packoffset(c2.w);
}

cbuffer cbuff2 : register(b2)
{
    matrix vpmat;
    float3 cameraPos;
}

cbuffer cbuff3 : register(b1)
{
    matrix billboard;
}

struct VSOutput
{
    float4 svpos : SV_POSITION;
    float4 worldpos : POSITION;
    float scale : SCALE;
    float4 color : COLOR;
};

struct GSOutput
{
    float4 svpos : SV_POSITION;
    float2 uv : TEXCOORD;
    float4 color : COLOR;
};