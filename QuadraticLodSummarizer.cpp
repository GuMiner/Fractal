#include <glm\gtx\norm.hpp>
#include "QuadraticLodSummarizer.h"

QuadraticLodSummarizer::QuadraticLodSummarizer(glm::vec3 objectPosition, float visibilityDistance, float scalingFactor)
    : objectPosition(objectPosition), scalingFactor(scalingFactor),
      visiblityDistanceSqd(visibilityDistance * visibilityDistance)
{
}

glm::ivec3 QuadraticLodSummarizer::Summarize(glm::vec3 playerPosition)
{
    float distSqd = glm::distance2(objectPosition, playerPosition);
    int LOD = (int)((visiblityDistanceSqd - distSqd) * scalingFactor);
    if (visiblityDistanceSqd - distSqd > 0)
    {
        return glm::ivec3(LOD, 0, 0);
    }

    return glm::ivec3(0, 0, 0);
}
