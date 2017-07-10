#version 400 core

in vec2 fs_pos;

uniform mat4 cameraOrientation;
uniform float aspectRatio;
uniform float fovY;

out vec4 color;

// Render a basic ray-traced basic bumpy terrain.
float getHeight(vec2 point)
{
    return sin(point.x * 3) * sin(point.y * 3) - 2.0f;
}

vec3 getNormal(float height, vec2 point, float delta)
{
    float xDelta = getHeight(vec2(point.x - delta, point.y)) - getHeight(vec2(point.x + delta, point.y));
    float zDelta = getHeight(vec2(point.x, point.y - delta)) - getHeight(vec2(point.x, point.y + delta));
    return normalize(vec3(xDelta, 2.0f * delta, zDelta));
}

bool castRay(vec3 eye, vec3 dir, inout vec3 intersectionPoint)
{
    float delta = 0.01f;
    float viewDistance = 10.0f;
    for (float i = 0; i < viewDistance; i += delta)
    {
        vec3 currentPos = eye + dir * i;
        if (currentPos.y < getHeight(currentPos.xz))
        {
            // TODO implement a bit of gradient descent to more accurately find the intersection point.
            currentPos -= (dir * (delta / 2));
            float height = getHeight(currentPos.xz);
            intersectionPoint = vec3(currentPos.x, height, currentPos.z);
            return true;
        }
        
        // TODO make delta increase as the distance increases from the camera (linearly).
    }
    
    return false;
}

vec3 getColor(vec3 point)
{
    vec3 normal = getNormal(point.y, point.xz, 0.01f);
    float magnitude = (normal.x * normal.x + normal.y * normal.y) / 1.41;
    
    // TODO perform more fancy coloring and some actual lighting.
    return vec3(magnitude, magnitude, magnitude);
}

void main(void)
{
    vec2 screenPos = vec2(fs_pos.x * 2, fs_pos.y * 2) - vec2(1, 1);
    vec3 cameraPos = vec3(cameraOrientation[3].x, -cameraOrientation[3].y, -cameraOrientation[3].z);
    
    vec3 dir = vec3(screenPos.x * fovY * aspectRatio, screenPos.y * fovY, -1) * mat3(cameraOrientation);
    vec3 intersectionPoint;
    
    if (castRay(cameraPos, dir, intersectionPoint))
    {
        color = vec4(getColor(intersectionPoint), 1.0f);
    }
    else
    {
        color = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    }
}