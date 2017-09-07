#pragma once
#include <vector>
#include <glm\vec2.hpp>
#include "CoreGeometry.h"

// UV maps the provided geometry
class UvMapper
{
    // The max texture size we will generate
    int maxTextureSize;

    // The desired number of pixels we want per unit of triangle.
    float desiredPixelsPerUnit;

public:
    UvMapper(int maxTextureSize, float desiredPixelsPerUnit);

    // Maps each triangle in the provided geometry to a triplet of UV coordinates.
    std::vector<glm::vec2> MapToTexture(CoreGeometry coreGeometry);
};

