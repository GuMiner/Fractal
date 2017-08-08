#pragma once
#include "IObject.h"

// Renders a basic cube used for testing purposes.
class TestCube : public IObject
{
    glm::vec3 position;
public:
    TestCube();
    ~TestCube();

    // Inherited via IObject
    virtual std::string GetName() override;
    
    virtual void Update(float frameTime) override;
    virtual void Render(glm::mat4 & projectionMatrix) override;

    virtual glm::vec3 GetPosition() override;
    virtual void SetPosition(glm::vec3 position) override;
};

