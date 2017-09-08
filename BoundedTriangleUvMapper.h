#pragma once
#include "IUvMapper.h"

// Defines a basic UvMapper -- mostly pulled from the retired CodeGell project -- to 
//  map a series of triangles into squares and pack them on a texture.
// This is inefficient, but easy to demonstrate and provides a UV Mapper baseline.
class BoundedTriangleUvMapper : public IUvMapper
{
public:
    BoundedTriangleUvMapper();

    // Inherited via UvMapper
    virtual std::vector<glm::vec2> MapToTexturePlane(CoreGeometry coreGeometry, float* maxScaleFactorPtr) override;
};

