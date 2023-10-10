#include "Line.hlsli"

[maxvertexcount(2)]
void main(
	point VSOutput input[1],
	inout LineStream<GSOutput> output
)
{
    GSOutput element;
    {
        element.svpos = input[0].fromPos;
        element.svpos = mul(vpmat, element.svpos);
        element.color = input[0].color;
        output.Append(element);
    }
    
    {
        element.svpos = input[0].toPos;
       element.svpos = mul(vpmat, element.svpos);
        element.color = input[0].color;
        output.Append(element);
    }
}