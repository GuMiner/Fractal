#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm\vec4.hpp>
#include <glm\mat4x4.hpp>
#include "Instance.h"

class IStandardRenderer
{
public:
    virtual void StartRendering(const glm::mat4& projectionMatrix) = 0;
    virtual void SwapToGeometry(GLuint vao, int activeTextureOffset, GLuint textureId, int vertexCount) = 0;
    virtual void RenderInstance(const Instance& instance) = 0;
};