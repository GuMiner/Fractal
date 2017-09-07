#pragma once
#include "CoreGeometry.h"
#include "IShape.h"

// Defines a cube from (-1, -1, -1) to (1, 1, 1)
class CubeShape : public IShape
{
    // The number of divisions per edge of the cube.
    float divisions;

public:
    CubeShape(float divisions);
    virtual CoreGeometry Generate() override;
};

