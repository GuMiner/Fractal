#pragma once
#include "CoreGeometry.h"
#include "EllipseShape.h"

// Defines a basic sphere at (0, 0, 0), r=1
class SphereShape : public EllipseShape
{
    // The radians for each triangle we generate.
    float angularResolution;

public:
    SphereShape(float angularResolution);
};

