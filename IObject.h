#pragma once
#include <glm\mat4x4.hpp>

// Defines a generically-renderable object.
class IObject
{
public:
    virtual void Update(float frameTime) = 0;
    virtual void Render(glm::mat4& projectionMatrix) = 0;
};