#pragma once
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm\vec3.hpp>
#include <glm\vec4.hpp>
#include "vertex\UvVbo.hpp"
#include "CoreGeometry.h"
#include "Texture.h"
#include "IStandardRenderer.h"
#include "IPerformanceProfiler.h"

// Defines the raw geometry that is sent to OpenGL.
class Geometry
{
    GLuint vao;
    
    Texture texture;
    UvVbo uvVbo;
    CoreGeometry coreGeometry;
    int vertexCount;

    bool sentToOpenGl;
    bool isGenerated;

public:
    long long geometryId;

    Geometry();
    void SetGeometryData(int width, int height, std::vector<unsigned char>& textureData, 
        CoreGeometry& coreGeometry, std::vector<glm::vec2>& uvs);
    void SetAsGenerated();
    
    bool CanSendToGpu() const;
    void SendToGpu(int activeTextureOffset, IPerformanceProfiler* profiler);

    // Swaps to this geometry and renders all instances of it to the GPU.
    void Render(std::vector<Instance*> instances, IStandardRenderer* standardRenderer, IPerformanceProfiler* profiler) const;
    ~Geometry();
};

