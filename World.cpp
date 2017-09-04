#include <imgui\imgui.h>
#include "logging\Logger.h"
#include "WireCube.h"
#include "StandardGeometryRenderer.h"
#include "World.h"

World::World()
    : geometryGenerationScheduler(), standardRenderer(new StandardGeometryRenderer()), objectLoader(&geometryGenerationScheduler)
{
    baseObjectTypes.push_back(new WireCube(&geometryGenerationScheduler));

    // Add a series of test cubes.
    BaseObject* testCube = new BaseObject();
    testCube->objectPosition = glm::vec3(0, 0, 0);

    baseObjectTypes[0]->AddObject(testCube);

    testCube = new BaseObject();
    testCube->objectPosition = glm::vec3(10, 0, 0);

    baseObjectTypes[0]->AddObject(testCube);

    testCube = new BaseObject();
    testCube->objectPosition = glm::vec3(20, 0, 0);

    baseObjectTypes[0]->AddObject(testCube);
}

void World::RenderPerformancePane(glm::vec4 performanceMetrics)
{
    ImGui::Begin("Performance", nullptr, ImVec2(100, 100), 0.50f);
    ImGui::SetCursorPos(ImVec2(5, 20));
    ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Geometry Jobs: %i", geometryGenerationScheduler.GetGeometryJobCount());
    ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Vertx Render Limit"); ImGui::SameLine();
    ImGui::ProgressBar(performanceMetrics.x);
    ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Texel Render Limit"); ImGui::SameLine();
    ImGui::ProgressBar(performanceMetrics.y);
    ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Vertx Transfer Limit"); ImGui::SameLine();
    ImGui::ProgressBar(performanceMetrics.z);
    ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Texel Transfer Limit"); ImGui::SameLine();
    ImGui::ProgressBar(performanceMetrics.w);
    ImGui::End();
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

void World::Update(glm::vec3 playerPosition, float gameTime, float frameTime)
{
    // TODO: We really want to not scan for all objects when we only have a single instance of the object in the game.
    // The scan will probably need to be position-based (we still have a list of object types, but we don't update / render all per-frame.
    for (BaseObjectType* baseObjectType : baseObjectTypes)
    {
        baseObjectType->Update(playerPosition, gameTime, frameTime);
    }
}

void World::Render(const glm::mat4& projectionMatrix)
{
    objectLoader.Render();

    performanceProfiler->ResetProfileFrame();
    standardRenderer->StartRendering(projectionMatrix);
    for (BaseObjectType* baseObjectType : baseObjectTypes)
    {
        baseObjectType->Render(standardRenderer, performanceProfiler);
    }

    RenderPerformancePane(performanceProfiler->GetPerformancePercentages());
}

World::~World()
{
    // Get rid of all possible archetypes
    for (BaseObjectType* type : baseObjectTypes)
    {
        delete type;
    }

    delete standardRenderer;
}
