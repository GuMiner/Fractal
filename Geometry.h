#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm\vec4.hpp>
#include "vertex\ColorVbo.hpp"
#include "vertex\NormalVbo.hpp"
#include "vertex\PositionVbo.hpp"
#include "Instance.h"
#include "Texture.h"
#include "IStandardRenderer.h"

// Defines the raw geometry that is sent to OpenGL.
class Geometry
{
    GLuint vao;
    
    Texture texture;
    ColorVbo colorVbo;
    NormalVbo normalVbo;
    PositionVbo positionVbo;
    int vertexCount;

    std::vector<Instance> instances;

    bool sentToOpenGl;

public:
    Geometry();

    // Swaps to this geometry and renders all isntances of it to the GPU.
    void Render(IStandardRenderer* standardRenderer);
    int GetVertexCount() const;
    ~Geometry();
};

