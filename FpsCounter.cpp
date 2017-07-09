#include <imgui\imgui.h>
#include "FpsCounter.h"

FpsCounter::FpsCounter() :
    fpsTimeAggregated(0.0f), fpsFramesCounted(0), lastFrameRate(60.0f)
{
}

void FpsCounter::UpdateFps(float frameTime)
{
    ++fpsFramesCounted;
    fpsTimeAggregated += frameTime;
    if (fpsTimeAggregated > 0.50f)
    {
        lastFrameRate = (float)fpsFramesCounted / (float)fpsTimeAggregated;
        fpsTimeAggregated = 0;
        fpsFramesCounted = 0;
    }
}

void FpsCounter::Render()
{
    ImGui::Begin("Stats", nullptr, ImVec2(100, 100), 0.50f);
    ImGui::SetCursorPos(ImVec2(5, 20));
    ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%.1f", lastFrameRate);
    ImGui::End();
}
