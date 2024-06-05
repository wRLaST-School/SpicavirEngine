#ifndef __SHADER_UTIL__
#define __SHADER_UTIL__
#include <ShaderMathUtil.hlsli>

//0.f～1.fの乱数を生成
float Random(float2 uv, int seed)
{
    return frac(sin(dot(uv.xy, float2(12.9898, 78.233)) + seed) * 43758.5453);
}

//0.f～1.fの乱数を生成 シードを指定しない
float Random(float2 uv)
{
    return frac(sin(dot(uv.xy, float2(12.9898, 78.233))) * 43758.5453);
}

float2 WorldToScreen(float3 worldPos, matrix viewProj, matrix viewport)
{
    return mul(mul(float4(worldPos, 1.0f), viewProj), viewport);
}

struct MouseRay
{
    float3 dir; // nearからfarへのベクトル
    float3 near; // 深度がnearClipの場合の座標
    float3 far; // 深度がfarClipの場合の座標
};

MouseRay ScreenToWorld(float2 screenPos, matrix viewProj, matrix viewport)
{
    MouseRay ray;
    
    matrix invVPort = inverse(viewport);
    matrix invVProj = inverse(viewProj);
    
    ray.near = mul(mul(float4(screenPos, 0.f, 1.f), invVPort), invVProj);
    ray.far = mul(mul(float4(screenPos, 1.f, 1.f), invVPort), invVProj);
    
    ray.dir = ray.far - ray.near;
    
    return ray;
}

#endif