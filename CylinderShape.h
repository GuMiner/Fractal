#pragma once
#include "CoreGeometry.h"
#include "IShape.h"

// Defines a cylinder with r=1 (configurable) from -1 to 1.
class CylinderShape : public IShape
{
    // Defines the resolution for each side of the cylinder
    float angularResolution;
    float lowerRadius;
    float upperRadius;

public:
    CylinderShape(float angularResolution);
    CylinderShape(float angularResolution, float lowerRadius, float upperRadius);
    virtual CoreGeometry Generate() override;
};

