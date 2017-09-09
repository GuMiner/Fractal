#pragma once
#include <vector>
#include <glm\vec3.hpp>
#include <glm\mat3x3.hpp>

// Defines generation for a plane.
class PlanePrimitive
{
public:
    // Adds a plane to the vertices / normals from (-1, -1, -1) to (1, 1, -1), with the inside facing towards (0, 0, 1).
    // To modify the plane, you can pass in the translation, and then the rotate/scale transform.
    static void AddPlane(std::vector<glm::vec3>& vertices, std::vector<glm::vec3>& normals, glm::mat3 rotateScaleTransform = glm::mat3(), glm::vec3 translation = glm::vec3());
};
