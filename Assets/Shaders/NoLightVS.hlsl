#include "NoLight.hlsli"

struct SkinOutput
{
    float4 pos;
    float3 normal;
};

struct VSInput
{
    float4 pos : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
    uint4 boneIndices : BONEINDICES;
    float4 boneWeights : BONEWEIGHTS;
};

SkinOutput ComputeSkin(VSInput input)
{
    SkinOutput output = (SkinOutput) 0;
	
    uint bIndex;
    float weight;
    matrix m;

    //bone 0
    bIndex = input.boneIndices.x;
    weight = input.boneWeights.x;
    m = bMatrix[bIndex];
    output.pos += weight * mul(m, input.pos);
    output.normal += weight * mul((float3x3) m, input.normal);
	
    //bone 1
    bIndex = input.boneIndices.y;
    weight = input.boneWeights.y;
    m = bMatrix[bIndex];
    output.pos += weight * mul(m, input.pos);
    output.normal += weight * mul((float3x3) m, input.normal);
	
    //bone 2
    bIndex = input.boneIndices.z;
    weight = input.boneWeights.z;
    m = bMatrix[bIndex];
    output.pos += weight * mul(m, input.pos);
    output.normal += weight * mul((float3x3) m, input.normal);
	
    //bone 3
    bIndex = input.boneIndices.w;
    weight = input.boneWeights.w;
    m = bMatrix[bIndex];
    output.pos += weight * mul(m, input.pos);
    output.normal += weight * mul((float3x3) m, input.normal);
	
    return output;
}

VSOutput main(VSInput input)
{
	//float3 lightDir = normalize(float3(1, -1, 1));
	//float3 lightColor = float3(1.0f, 1.0f, 1.0f);
    
    SkinOutput skinned = ComputeSkin(input);

    float4 wnormal = normalize(mul(mat, float4(skinned.normal, 0)));
    //float4 wnormal = float4(normal, 0);
    float4 wpos = mul(mat, skinned.pos);

    VSOutput output;
    output.svpos = mul(mul(vpmat, mat), skinned.pos);
    output.worldpos = wpos;
    output.normal = wnormal.xyz;
    output.uv = input.uv;

    return output;
}