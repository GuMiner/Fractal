#include <iostream>
#include <sstream>
#include <imgui.h>
#include "../Time.h"
#include "FpsCounter.h"

FpsCounter::FpsCounter() :
    timeTotal(0.1f), frameTotal(1) {
}

void FpsCounter::Update() {
    ++frameTotal;
    timeTotal += Time::GlobalTime->LastFrameInterval();

    // Only update every half second
    if (timeTotal > 0.50f)
    {
        lastFrameRateToRender = (float)frameTotal / (float)timeTotal;
        frameTotal = 0;
        timeTotal = 0.0f;
    }
}

void FpsCounter::Render() {
    ImGui::Begin("FPS", nullptr);
    ImGui::SetWindowSize(ImVec2(50, 50), ImGuiCond_Once);
    ImGui::SetWindowPos(ImVec2(10, 130), ImGuiCond_Once);
    ImGui::SetCursorPos(ImVec2(5, 20));
    ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%.0f", lastFrameRateToRender);
    ImGui::End();
}
