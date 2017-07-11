#version 400 core

in vec2 fs_pos;

uniform vec3 cameraPosition;
uniform vec3 lookAt;
uniform vec3 up;
uniform float aspectRatio;
uniform float fovY;

out vec4 color;

// Render a basic ray-traced basic bumpy terrain.
float getHeight(vec2 point)
{
    return sin(point.x * 3) * sin(point.y * 3) * 0.5f - 1.0f;
}

vec3 getNormal(float height, vec2 point, float delta)
{
    float xDelta = getHeight(vec2(point.x - delta, point.y)) - getHeight(vec2(point.x + delta, point.y));
    float zDelta = getHeight(vec2(point.x, point.y - delta)) - getHeight(vec2(point.x, point.y + delta));
    return normalize(vec3(xDelta, 2.0f * delta, zDelta));
}

bool castRay(vec3 eye, vec3 dir, inout vec3 intersectionPoint)
{
    // This binary search doesn't work as our surface is not flat.
    // const float MaxErr = 0.001f; // When we stop iterating
    // 
    // intersectionPoint = eye + dir * 0.10f;
    // float step = 1000.0f; // Set to max
    // while (step > MaxErr)
    // {
    //     vec3 middlePos = intersectionPoint + dir * (step / 2);
    //     float height = getHeight(middlePos.xz);
    //     
    //     if (middlePos.y > height)
    //     {
    //         intersectionPoint = middlePos;
    //     }
    //     
    //     step = step / 2;
    // }
    // return true;
    
    float minMax = 1000.0f;
    vec3 maxPos = eye + dir * minMax;
    while (maxPos.y > getHeight(maxPos.xz) && minMax > 10.0f)
    {
        minMax /= 2.0f;
        maxPos = eye + dir * minMax;
    }
    
    minMax *= 2.0f;
    
    float delta = 0.01f;
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
        
        // TODO slowly increase delta as we move laong.
        delta *= 1.002f;
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

// TODO simplify
mat3 rotationMatrix(vec3 axis, float angle)
{
    vec3 s = axis * sin(angle);
    float c = cos(angle);
    vec3 oc = axis * (1 - c);
    
    vec3 sqd = axis * axis;
    vec3 cross = oc * sqd + c;
    
    return mat3(cross.x,             oc.x * axis.y - s.z, oc.z * axis.x + s.y, 
                oc.x * axis.y + s.z, cross.y,             oc.y * axis.z - s.x, 
                oc.z * axis.x - s.y, oc.y * axis.z + s.x, cross.z);
}

void main(void)
{
    vec2 screenPos = vec2(fs_pos.x * 2, fs_pos.y * 2) - vec2(1, 1);
    
    // Our actual direction is the lookAt location rotated by our current pixel position.
    float aspectRatioFactor = 4;
    vec2 rotationAngles = vec2(screenPos.x * fovY * aspectRatio / aspectRatioFactor, -screenPos.y * fovY / aspectRatioFactor);
    
    vec3 right = cross(up, lookAt);
    vec3 rotatedX = lookAt * rotationMatrix(up, radians(rotationAngles.x));
    vec3 dir = rotatedX * rotationMatrix(right, radians(rotationAngles.y));
    vec3 intersectionPoint;
    
    if (castRay(cameraPosition, dir, intersectionPoint))
    {
        color = vec4(getColor(intersectionPoint), 1.0f);
    }
    else
    {
        color = vec4(0.0f, 0.0f, 0.10f, 1.0f);
    }
}