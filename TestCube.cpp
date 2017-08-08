#include <glm\vec3.hpp>
#include "TestCube.h"

TestCube::TestCube() : position(glm::vec3(0, 0, 0))
{
    // TODO: I'll need some test objects for this idea, but they do need to use the new rendering structure.
    // // Define a cube going from black (X-, Y-, Z-) to RGB (X+, Y+, Z+), (-1, -1, -1) to (1, 1, 1)
    // 
    // // 3 lines from -1, -1, -1
    // renderer->AddLine(glm::vec3(-1, -1, -1), glm::vec3(0, 0, 0), glm::vec3(1, -1, -1), glm::vec3(1, 0, 0));
    // renderer->AddLine(glm::vec3(-1, -1, -1), glm::vec3(0, 0, 0), glm::vec3(-1, 1, -1), glm::vec3(0, 1, 0));
    // renderer->AddLine(glm::vec3(-1, -1, -1), glm::vec3(0, 0, 0), glm::vec3(-1, -1, 1), glm::vec3(0, 0, 1));
    // 
    // // Intermediate lines
    // renderer->AddLine(glm::vec3(1, 1, -1), glm::vec3(1, 1, 0), glm::vec3(1, -1, -1), glm::vec3(1, 0, 0));
    // renderer->AddLine(glm::vec3(-1, 1, 1), glm::vec3(0, 1, 1), glm::vec3(-1, 1, -1), glm::vec3(0, 1, 0));
    // renderer->AddLine(glm::vec3(1, -1, 1), glm::vec3(1, 0, 1), glm::vec3(-1, -1, 1), glm::vec3(0, 0, 1));
    // 
    // renderer->AddLine(glm::vec3(1, -1, 1), glm::vec3(1, 0, 1), glm::vec3(1, -1, -1), glm::vec3(1, 0, 0));
    // renderer->AddLine(glm::vec3(1, 1, -1), glm::vec3(1, 1, 0), glm::vec3(-1, 1, -1), glm::vec3(0, 1, 0));
    // renderer->AddLine(glm::vec3(-1, 1, 1), glm::vec3(0, 1, 1), glm::vec3(-1, -1, 1), glm::vec3(0, 0, 1));
    // 
    // // 3 lines from 1, 1, 1
    // renderer->AddLine(glm::vec3(1, 1, -1), glm::vec3(1, 1, 0), glm::vec3(1, 1, 1), glm::vec3(1, 1, 1));
    // renderer->AddLine(glm::vec3(-1, 1, 1), glm::vec3(0, 1, 1), glm::vec3(1, 1, 1), glm::vec3(1, 1, 1));
    // renderer->AddLine(glm::vec3(1, -1, 1), glm::vec3(1, 0, 1), glm::vec3(1, 1, 1), glm::vec3(1, 1, 1));
    // 
    // renderer->Update();
}

TestCube::~TestCube()
{
    // delete renderer;
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
    // renderer->Render(projectionMatrix);
}

glm::vec3 TestCube::GetPosition()
{
    return position;
}

void TestCube::SetPosition(glm::vec3 newPosition)
{
    glm::vec3 difference = newPosition - position;
    // for (size_t i = 0; i < renderer->positionBuffer.vertices.size(); i++)
    // {
    //     renderer->positionBuffer.vertices[i] += difference;
    // }
    // 
    // renderer->Update();
    position = newPosition;
}
