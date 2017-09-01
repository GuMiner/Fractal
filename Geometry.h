#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm\vec4.hpp>
#include "vertex\ColorVbo.hpp"
#include "vertex\NormalVbo.hpp"
#include "vertex\PositionVbo.hpp"
#include "Texture.h"
#include "IStandardRenderer.h"
#include "IPerformanceProfiler.h"

// Defines the raw geometry that is sent to OpenGL.
class Geometry
{
    GLuint vao;
    
    Texture texture;
    ColorVbo colorVbo;
    NormalVbo normalVbo;
    PositionVbo positionVbo;
    int vertexCount;

    bool sentToOpenGl;
    bool isGenerated;

public:
    // The number of instances that are referring to this geometry.
    unsigned int InstanceReferences;
    unsigned int geometryId;

    Geometry();

    // Swaps to this geometry and renders all isntances of it to the GPU.
    void Render(std::vector<Instance*> instances, IStandardRenderer* standardRenderer, IPerformanceProfiler* profiler) const;
    ~Geometry();
};

