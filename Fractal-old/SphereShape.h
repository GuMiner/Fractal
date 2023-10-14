#pragma once
#include "CoreGeometry.h"
#include "IShape.h"

// Defines a basic sphere at (0, 0, 0), r=1
class SphereShape : public IShape
{
protected:
    int subdivisions;

public:
    // Generates a sphere with a series of subdivisions.
    // 1 == tetrahedron. 2 = tetrahedron with subdivided portions.
    SphereShape(int subdivisions);
    virtual CoreGeometry Generate() override;
};

