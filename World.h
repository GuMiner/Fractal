#pragma once
#include <glm\mat4x4.hpp>
#include "shaders\ShaderFactory.h"
#include "GeometryGenerationScheduler.h"
#include "IPerformanceProfiler.h"
#include "IStandardRenderer.h"
#include "OpenGlCapabilities.h"

// Holds objects and their means of entering and leaving existence.
// Eventually this will be broken apart into scenes, scenarios, maps, etc.
class World
{
    GeometryGenerationScheduler geometryGenerationScheduler;
    IPerformanceProfiler* performanceProfiler;
    IStandardRenderer* standardRenderer;

    // Renders performance metrics (see IPerformanceProfiler for the format) in a pane.
    void RenderPerformancePane(glm::vec4 performanceMetrics);

public:
    World();
    bool LoadGraphics(OpenGlCapabilities capabilities, IPerformanceProfiler* performanceProfiler, ShaderFactory* shaderFactory);
    void Render(const glm::mat4& projectionMatrix);
    ~World();
};

