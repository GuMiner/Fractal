#pragma once
#include <glm\vec3.hpp>
#include "ILodSummarizer.h"

// Summarizes a player position to a quadratic LOD
class QuadraticLodSummarizer : public ILodSummarizer
{
    glm::vec3 objectPosition;
    float visiblityDistanceSqd;
    float scalingFactor;

public:
    QuadraticLodSummarizer(glm::vec3 objectPosition, float visibilityDistance, float scalingFactor);

    // Returns ivec3(LOD, 0, 0), where LOD == 0 if the object is outside the visiblity distance, 
    virtual glm::ivec3 Summarize(glm::vec3 playerPosition) override;
};

