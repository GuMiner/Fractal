#pragma once
#include "BaseObjectType.h"

// Renders a basic cube used for testing purposes.
class WireCube : public BaseObjectType
{
    glm::vec3 position;
public:
    WireCube();
    ~WireCube();

    virtual std::string GetName() override;
};

