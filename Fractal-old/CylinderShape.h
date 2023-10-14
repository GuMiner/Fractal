#pragma once
#include "CoreGeometry.h"
#include "IShape.h"

// Defines a cylinder with r=1 from h=-1 to 1.
class CylinderShape : public IShape
{
    // Defines the resolution for each side of the cylinder
    int angularSteps;

public:
    CylinderShape(int angularSteps);
    virtual CoreGeometry Generate() override;
};

