#include <glm\vec3.hpp>
#include "TestCube.h"

TestCube::TestCube() : renderer(new LineRenderer(false))
{
    // Define a cube going from black (X-, Y-, Z-) to RGB (X+, Y+, Z+), (-1, -1, -1) to (1, 1, 1)

    // 3 lines from -1, -1, -1
    renderer->AddLine(glm::vec3(-1, -1, -1), glm::vec3(0, 0, 0), glm::vec3(1, -1, -1), glm::vec3(1, 0, 0));
    renderer->AddLine(glm::vec3(-1, -1, -1), glm::vec3(0, 0, 0), glm::vec3(-1, 1, -1), glm::vec3(0, 1, 0));
    renderer->AddLine(glm::vec3(-1, -1, -1), glm::vec3(0, 0, 0), glm::vec3(-1, -1, 1), glm::vec3(0, 0, 1));

    // Intermediate lines
    renderer->AddLine(glm::vec3(1, 1, -1), glm::vec3(1, 1, 0), glm::vec3(1, -1, -1), glm::vec3(1, 0, 0));
    renderer->AddLine(glm::vec3(-1, 1, 1), glm::vec3(0, 1, 1), glm::vec3(-1, 1, -1), glm::vec3(0, 1, 0));
    renderer->AddLine(glm::vec3(1, -1, 1), glm::vec3(1, 0, 1), glm::vec3(-1, -1, 1), glm::vec3(0, 0, 1));

    renderer->AddLine(glm::vec3(1, -1, 1), glm::vec3(1, 0, 1), glm::vec3(1, -1, -1), glm::vec3(1, 0, 0));
    renderer->AddLine(glm::vec3(1, 1, -1), glm::vec3(1, 1, 0), glm::vec3(-1, 1, -1), glm::vec3(0, 1, 0));
    renderer->AddLine(glm::vec3(-1, 1, 1), glm::vec3(0, 1, 1), glm::vec3(-1, -1, 1), glm::vec3(0, 0, 1));

    // 3 lines from 1, 1, 1
    renderer->AddLine(glm::vec3(1, 1, -1), glm::vec3(1, 1, 0), glm::vec3(1, 1, 1), glm::vec3(1, 1, 1));
    renderer->AddLine(glm::vec3(-1, 1, 1), glm::vec3(0, 1, 1), glm::vec3(1, 1, 1), glm::vec3(1, 1, 1));
    renderer->AddLine(glm::vec3(1, -1, 1), glm::vec3(1, 0, 1), glm::vec3(1, 1, 1), glm::vec3(1, 1, 1));

    renderer->Update();
}

TestCube::~TestCube()
{
    delete renderer;
}

std::string TestCube::GetName()
{
    return "Test Cube";
}

void TestCube::Update(float frameTime)
{
    // TestCube does nothing.
}

void TestCube::Render(glm::mat4 & projectionMatrix)
{
    renderer->Render(projectionMatrix);
}
