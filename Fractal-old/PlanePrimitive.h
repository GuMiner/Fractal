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

    // Adds a plane fan with <segments> segments at (0, 0, 0) with normals facing towards (0, 1, 0).
    // The fan runs from (0, 0, 0) to (1, 0, 0), with the first line parallel to the x+ axis, rotating CCW
    static void AddPlaneFan(std::vector<glm::vec3>& vertices, std::vector<glm::vec3>& normals, int segments, glm::mat4 transform = glm::mat4());
};

