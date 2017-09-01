#pragma once
#include <glm\vec3.hpp>

// Defines a level of detail used for finding existing geometry
class Lod
{
public:
    glm::vec3 position;

    // Generates a new LOD from the given position
    Lod(const glm::vec3 position);
};

