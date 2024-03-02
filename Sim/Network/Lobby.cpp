#include <sstream>
#include <imgui.h>
#include "../Time.h"
#include "Lobby.h"

Lobby::Lobby() {
    buttonCount = 0;
}

void Lobby::Render() {
    ImGui::Begin("Lobby", nullptr);

    ImGui::SetWindowPos(ImVec2(300, 10), ImGuiCond_Once);
    ImGui::SetWindowSize(ImVec2(280, 200), ImGuiCond_Once);
    ImGui::SetCursorPos(ImVec2(5, 20));

    if (ImGui::Button("Begin server sync")) {
        ImGui::Text("Test bounceback");
        ++buttonCount;
    }

    std::stringstream time;
    time << Time::GlobalTime->RunTime() << " " << buttonCount;
    ImGui::Text(time.str().c_str());
    ImGui::End();
}