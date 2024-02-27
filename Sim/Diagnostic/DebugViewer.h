#pragma once
#include <glm\vec3.hpp>

class DebugViewer {
public:
    static DebugViewer* Global;

    glm::vec3 WaterColor;
    float waterGrassCutoff;
    glm::vec3 GrassColor;
    float grassForestCutoff;
    glm::vec3 ForestColor;
    float forestTundraCutoff;
    glm::vec3 TundraColor;
    float tundraRockCutoff;
    glm::vec3 RockColor;
    float rockSnowCutoff;
    glm::vec3 SnowColor;

    DebugViewer();
    void Update();
    void Render();
};

