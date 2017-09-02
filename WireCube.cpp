#include <glm\vec3.hpp>
#include "WireCube.h"

WireCube::WireCube() : position(glm::vec3(0, 0, 0))
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

WireCube::~WireCube()
{
}

std::string WireCube::GetName()
{
    return "Test Cube";
}