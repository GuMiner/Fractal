#pragma once
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

public:
    World();
    bool LoadGraphics(OpenGlCapabilities capabilities, IPerformanceProfiler* performanceProfiler, ShaderFactory* shaderFactory);
    ~World();
};

