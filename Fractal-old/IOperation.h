#pragma once
#include "CoreGeometry.h"

// Defines how to perform a CSG operation
class IOperation
{
public:
    virtual CoreGeometry Perform(CoreGeometry first, CoreGeometry second) = 0;
};

