#pragma once
#include "IObject.h"

class TestGrid : public IObject
{
public:
    TestGrid();
    ~TestGrid();

    // Inherited via IObject
    virtual std::string GetName() override;
    virtual void Update(float frameTime) override;
    virtual void Render(glm::mat4 & projectionMatrix) override;
};

