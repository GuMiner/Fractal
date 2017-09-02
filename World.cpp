#include "logging\Logger.h"
#include "StandardGeometryRenderer.h"
#include "World.h"

World::World()
    : geometryGenerationScheduler(), standardRenderer(new StandardGeometryRenderer())
{
}

bool World::LoadGraphics(OpenGlCapabilities capabilities, IPerformanceProfiler* performanceProfiler, ShaderFactory* shaderFactory)
{
    // We leave 1 thread free for the rest of the game engine.
    geometryGenerationScheduler.StartThreads(capabilities.logicalCoreCount - 1);
    
    this->performanceProfiler = performanceProfiler;

    if (!((StandardGeometryRenderer*)standardRenderer)->LoadProgram(shaderFactory))
    {
        Logger::LogError("Unable to load the standard geometry renderer!");
        return false;
    }

    return true;
}


World::~World()
{
    delete standardRenderer;
}
