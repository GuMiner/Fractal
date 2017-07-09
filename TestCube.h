#pragma once
#include "IObject.h"
#include "LineRenderer.h"

// Renders a basic cube used for testing purposes.
class TestCube : public IObject
{
    LineRenderer* renderer;

public:
    TestCube();
    ~TestCube();

    // Inherited via IObject
    virtual std::string GetName() override;
    virtual void Update(float frameTime) override;
    virtual void Render(glm::mat4 & projectionMatrix) override;
};

