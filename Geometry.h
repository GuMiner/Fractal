#pragma once
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm\vec3.hpp>
#include <glm\vec4.hpp>
#include "vertex\UvVbo.hpp"
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
    UvVbo uvVbo;
    NormalVbo normalVbo;
    PositionVbo positionVbo;
    int vertexCount;

    bool sentToOpenGl;
    bool isGenerated;

public:
    long long geometryId;

    Geometry();
    void SetGeometryData(int width, int height, std::vector<unsigned char>& textureData, 
        std::vector<glm::vec3>& vertices, std::vector<glm::vec3>& normals, std::vector<glm::vec2>& uvs);
    void SetAsGenerated();
    
    bool CanSendToGpu() const;
    void SendToGpu();

    // Swaps to this geometry and renders all instances of it to the GPU.
    void Render(std::vector<Instance*> instances, IStandardRenderer* standardRenderer, IPerformanceProfiler* profiler) const;
    ~Geometry();
};

