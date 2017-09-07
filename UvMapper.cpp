#include "UvMapper.h"

UvMapper::UvMapper(int maxTextureSize, float desiredPixelsPerUnit)
    : maxTextureSize(maxTextureSize), desiredPixelsPerUnit(desiredPixelsPerUnit)
{
}

std::vector<glm::vec2> UvMapper::MapToTexture(CoreGeometry coreGeometry)
{
    std::vector<glm::vec2> uvCoordinates;

    // TODO run through a variety of algorithms and output the best result.
    return uvCoordinates;
}
