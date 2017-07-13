#version 400 core
#include "fullScreenQuad/rayComputation.fs_frag"

uniform vec3 cameraPosition;

out vec4 color;

const float maxDistance = 500.0f;
const vec3 terrainColor = vec3(0.2f, 0.2f, 0.2f);
const vec3 sunPosition = vec3(0.0f, 0.0f, 10.0f);
const vec3 sunColor = vec3(1.0f, 0.85f, 0.80f);
const vec3 fogColor = vec3(0.53f, 0.6f, 0.65f);

// Render a basic ray-traced basic bumpy terrain.
float getHeight(vec2 point)
{
    return sin(point.x * 3) * sin(point.y * 3) * 0.5f - 1.0f + cos(point.x * 10) * 0.25f;
}

vec4 applyGammaCorrection(vec4 color)
{
    return vec4(pow(color.xyz, vec3(0.4545)), 1.0);
}

vec3 getNormal(float height, vec2 point, float delta)
{
    float xDelta = getHeight(vec2(point.x - delta, point.y)) - getHeight(vec2(point.x + delta, point.y));
    float zDelta = getHeight(vec2(point.x, point.y - delta)) - getHeight(vec2(point.x, point.y + delta));
    return normalize(vec3(xDelta, 2.0f * delta, zDelta));
}

vec3 mixFog(vec3 color, float distance)
{
    const float exponentialFactor = -0.01f;
    float fogAmount = 1 - exp(distance * exponentialFactor);
    
    return mix(color, fogColor, fogAmount);
}

bool castRay(vec3 eye, vec3 dir, inout vec3 intersectionPoint)
{
    float minMax = maxDistance;
    vec3 maxPos = eye + dir * minMax;
    while (maxPos.y > getHeight(maxPos.xz) && minMax > 10.0f)
    {
        minMax /= 2.0f;
        maxPos = eye + dir * minMax;
    }
    
    minMax *= 2.0f;
    
    float delta = 0.006f;
    for (float i = 0.10f; i < minMax; i += delta)
    {
        vec3 currentPos = eye + dir * i;
        float height = getHeight(currentPos.xz);
        intersectionPoint = vec3(currentPos.x, height, currentPos.z);
        if (currentPos.y < height)
        {
            // TODO implement a bit of gradient descent to more accurately find the intersection point.
            currentPos -= (dir * (delta / 2));
            height = getHeight(currentPos.xz);
            intersectionPoint = vec3(currentPos.x, height, currentPos.z);
            return true;
        }
        
        delta *= 1.002f;
    }
}

void main(void)
{
    vec3 rayDirection = getRayDirection();
    
    vec3 intersectionPoint;
    bool didHitTerrain = castRay(cameraPosition, rayDirection, intersectionPoint);
    if (didHitTerrain)
    {
        float cameraDistance = distance(cameraPosition, intersectionPoint);
        vec3 sunDirection = normalize(intersectionPoint - sunPosition);
        vec3 normal = getNormal(intersectionPoint.y, intersectionPoint.xz, 0.01f);
        vec3 baseColor = max(0.0f, dot(sunDirection, normal)) * terrainColor;
        
        color = vec4(mixFog(baseColor, cameraDistance), 1.0f);
    }
    else
    {
        color = vec4(fogColor, 1.0f);
    }
    
    color = applyGammaCorrection(color);
}