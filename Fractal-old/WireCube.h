#pragma once
#include "BaseObjectType.h"

// Renders a basic cube used for testing purposes.
class WireCube : public BaseObjectType
{
public:
    WireCube(GeometryGenerationScheduler* scheduler);

    virtual std::string GetName() override;
};

