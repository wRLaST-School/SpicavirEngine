#include "Finalize.hlsli"

VSOutput main(float4 pos : POSITION, float2 uv : TEXCOORD)
{
    VSOutput output;
    output.svpos = pos;
    
    output.tex0 = uv;
    
    return output;
}