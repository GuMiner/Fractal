#pragma once
#include <vector>
#include <glm\vec2.hpp>
#include "CoreGeometry.h"

// UV maps the provided geometry from triangles onto a 2D texture
class IUvMapper
{
public:
    // Maps each triangle in the provided geometry to a triplet of UV coordinates.
    virtual std::vector<glm::vec2> MapToTexturePlane(CoreGeometry coreGeometry, float* maxScaleFactorPtr) = 0;
};

