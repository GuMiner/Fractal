#pragma once
#include <vector>
#include <glm\mat4x4.hpp>
#include "shaders\ShaderFactory.h"
#include "BaseObjectType.h"
#include "GeometryGenerationScheduler.h"
#include "IPerformanceProfiler.h"
#include "IStandardRenderer.h"
#include "ObjectLoader.h"
#include "OpenGlCapabilities.h"

// Holds objects and their means of entering and leaving existence.
// Eventually this will be broken apart into scenes, scenarios, maps, etc.
class World
{
    GeometryGenerationScheduler geometryGenerationScheduler;
    IPerformanceProfiler* performanceProfiler;
    IStandardRenderer* standardRenderer;

    // Lets us load in individual objects and infinite objects to test with
    ObjectLoader objectLoader;

    // The listing of object types we have that we can possibly render
    std::vector<BaseObjectType*> baseObjectTypes;

    // Renders performance metrics (see IPerformanceProfiler for the format) in a pane.
    void RenderPerformancePane(glm::vec4 performanceMetrics);

public:
    World();
    bool LoadGraphics(OpenGlCapabilities capabilities, IPerformanceProfiler* performanceProfiler, ShaderFactory* shaderFactory);
    void Update(float gameTime, float frameTime);
    void Render(const glm::mat4& projectionMatrix);
    ~World();
};

