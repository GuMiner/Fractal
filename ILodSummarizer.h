#pragma once
#include <glm\vec3.hpp>

// Defines how LOD is summarized into less-specific, integer-based LOD variants.
class ILodSummarizer
{
public:
    virtual glm::ivec3 Summarize(glm::vec3 playerPosition) = 0;
};

