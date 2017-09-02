#include <imgui\imgui.h>
#include "logging\Logger.h"
#include "StandardGeometryRenderer.h"
#include "World.h"

World::World()
    : geometryGenerationScheduler(), standardRenderer(new StandardGeometryRenderer())
{
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

void World::Render(const glm::mat4& projectionMatrix)
{
    performanceProfiler->ResetProfileFrame();
    standardRenderer->StartRendering(projectionMatrix);
    // TODO: render geometry

    RenderPerformancePane(performanceProfiler->GetPerformancePercentages());
}

World::~World()
{
    delete standardRenderer;
}
