#pragma once
#include "CoreGeometry.h"
#include "IShape.h"

// Defines a cube from (-1, -1, -1) to (1, 1, 1)
class CubeShape : public IShape
{
    // The number of divisions per edge of the cube.
    int divisions;

public:
    CubeShape(int divisions);
    virtual CoreGeometry Generate() override;
};

