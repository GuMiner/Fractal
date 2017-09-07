#pragma once
#include "CoreGeometry.h"
#include "IOperation.h"

class IntersectionOperation : public IOperation
{
public:
    virtual CoreGeometry Perform(CoreGeometry first, CoreGeometry second) override;
};

