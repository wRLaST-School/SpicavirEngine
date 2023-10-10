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
    matrix billboardMat;
    float3 cameraPos;
}

cbuffer cbuff1 : register(b1)
{
    matrix billboard;
}

struct VSOutput
{
    float4 fromPos : SV_POSITION;
    float4 toPos : POSITION;
    float4 color : COLOR;
};

struct GSOutput
{
    float4 svpos : SV_POSITION;
    float4 color : COLOR;
};