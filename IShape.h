#pragma once
#include "CoreGeometry.h"

// Defines how to generate a shape.
class IShape
{
public:
    virtual CoreGeometry Generate() = 0;
};