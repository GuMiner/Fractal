#include <imgui.h>
#include "DebugViewer.h"

DebugViewer* DebugViewer::Global;

DebugViewer::DebugViewer() {
    // Defaults
    WaterColor = glm::vec3(0.0f, 1.0f, 0.0f);
    waterGrassCutoff = 0.1f;
    GrassColor = glm::vec3(0.0f, 1.0f, 1.0f);
    grassForestCutoff = 0.2f;
    ForestColor = glm::vec3(1.0f, 1.0f, 0.0f);
    forestTundraCutoff = 0.3f;
    TundraColor = glm::vec3(0.0f, 1.0f, 1.0f);
    tundraRockCutoff = 0.4f;
    RockColor = glm::vec3(0.0f, 1.0f, 0.0f);
    rockSnowCutoff = 0.5f;
    SnowColor = glm::vec3(1.0f, 1.0f, 0.0f);
}

void DebugViewer::Update() {
    //
}

void DebugViewer::Render() {
    ImGui::Begin("Debug", nullptr);
    ImGui::SetWindowSize(ImVec2(280, 300), ImGuiCond_Once);
    ImGui::SetWindowPos(ImVec2(10, 50), ImGuiCond_Once);
    ImGui::SetCursorPos(ImVec2(5, 20));
    ImGui::ColorEdit3("Water", &WaterColor[0]);
    ImGui::SliderFloat("W <-> G", &waterGrassCutoff, 0.0f, grassForestCutoff);
    
    ImGui::ColorEdit3("Grass", &GrassColor[0]);
    ImGui::SliderFloat("G <-> F", &grassForestCutoff, waterGrassCutoff, forestTundraCutoff);

    ImGui::ColorEdit3("Forest", &ForestColor[0]);
    ImGui::SliderFloat("F <-> T", &forestTundraCutoff, grassForestCutoff, tundraRockCutoff);

    ImGui::ColorEdit3("Tundra", &TundraColor[0]);
    ImGui::SliderFloat("T <-> R", &tundraRockCutoff, forestTundraCutoff, rockSnowCutoff);

    ImGui::ColorEdit3("Rock", &RockColor[0]);
    ImGui::SliderFloat("R <-> S", &rockSnowCutoff, tundraRockCutoff, 1.0f);

    ImGui::ColorEdit3("Snow", &SnowColor[0]);
    ImGui::End();
}
