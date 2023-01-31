#include "BloomP2.hlsli"

VSOutput main(float4 pos : POSITION, float2 uv : TEXCOORD)
{
    VSOutput output;
    output.svpos = pos;
    
    float2 tex = uv;
    
    float2 texSize = float2(960, 1080);
    float level;
    //sceneTexture.GetDimensions(0, texSize.x, texSize.y, level);
    
    output.tex0.xy = float2(1.0f / texSize.x, 0.0f);
    output.tex1.xy = float2(3.0f / texSize.x, 0.0f);
    output.tex2.xy = float2(5.0f / texSize.x, 0.0f);
    output.tex3.xy = float2(7.0f / texSize.x, 0.0f);
    output.tex4.xy = float2(9.0f / texSize.x, 0.0f);
    output.tex5.xy = float2(11.0f / texSize.x, 0.0f);
    output.tex6.xy = float2(13.0f / texSize.x, 0.0f);
    output.tex7.xy = float2(15.0f / texSize.x, 0.0f);
    
    output.tex0.zw = output.tex0.xy * -1.0f;
    output.tex1.zw = output.tex1.xy * -1.0f;
    output.tex2.zw = output.tex2.xy * -1.0f;
    output.tex3.zw = output.tex3.xy * -1.0f;
    output.tex4.zw = output.tex4.xy * -1.0f;
    output.tex5.zw = output.tex5.xy * -1.0f;
    output.tex6.zw = output.tex6.xy * -1.0f;
    output.tex7.zw = output.tex7.xy * -1.0f;
    
    output.tex0 += float4(tex, tex);
    output.tex1 += float4(tex, tex);
    output.tex2 += float4(tex, tex);
    output.tex3 += float4(tex, tex);
    output.tex4 += float4(tex, tex);
    output.tex5 += float4(tex, tex);
    output.tex6 += float4(tex, tex);
    output.tex7 += float4(tex, tex);
    
    return output;
}