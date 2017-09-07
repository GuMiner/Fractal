#pragma once
#include "CoreGeometry.h"
#include "IShape.h"

// Defines a multi-directional ellipse
class EllipseShape : public IShape
{
    // The radians for each triangle we generate.
    float angularResolution;
    glm::vec3 radii;

public:
    EllipseShape(float angularResolution, glm::vec3 radii);
    virtual CoreGeometry Generate() override;
};

