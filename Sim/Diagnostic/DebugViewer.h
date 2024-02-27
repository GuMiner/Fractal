#pragma once
#include <glm\vec3.hpp>

class DebugViewer {
public:
    static DebugViewer* Global;

    glm::vec3 WaterColor;
    float WaterGrassCutoff;
    glm::vec3 GrassColor;
    float GrassForestCutoff;
    glm::vec3 ForestColor;
    float ForestTundraCutoff;
    glm::vec3 TundraColor;
    float TundraRockCutoff;
    glm::vec3 RockColor;
    float RockSnowCutoff;
    glm::vec3 SnowColor;

    DebugViewer();
    void Update();
    void Render();
};

