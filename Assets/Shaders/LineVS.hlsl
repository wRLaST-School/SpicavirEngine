#include "Line.hlsli"
VSOutput main( float4 from : FROM, float4 to : TO, float4 color : COLOR) 
{
    VSOutput output;
    output.fromPos = from;
    output.toPos = to;
    output.color = color;
	
	return output;
}