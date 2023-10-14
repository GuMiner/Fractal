#pragma once
#include "CoreGeometry.h"
#include "IShape.h"

class TorusShape : public IShape
{
    float angularResolution;
    float innerRadius;
    float outerRadius;

public:
    TorusShape(float angularResolution, float innerRadius, float outerRadius);
    virtual CoreGeometry Generate() override;
};

