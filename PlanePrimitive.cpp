#include "PlanePrimitive.h"

void PlanePrimitive::AddPlane(std::vector<glm::vec3>& vertices, std::vector<glm::vec3>& normals, glm::mat3 rotateScaleTransform, glm::vec3 translation)
{
    // Add a basic plane
    vertices.push_back(glm::vec3(-1, -1, -1));
    vertices.push_back(glm::vec3(1, -1, -1));
    vertices.push_back(glm::vec3(1, 1, -1));

    vertices.push_back(glm::vec3(-1, -1, -1));
    vertices.push_back(glm::vec3(1, 1, -1));
    vertices.push_back(glm::vec3(-1, 1, -1));

    normals.push_back(glm::vec3(0, 0, -1));
    normals.push_back(glm::vec3(0, 0, -1));
    normals.push_back(glm::vec3(0, 0, -1));

    normals.push_back(glm::vec3(0, 0, -1));
    normals.push_back(glm::vec3(0, 0, -1));
    normals.push_back(glm::vec3(0, 0, -1));

    // Lookback six items and rotate as applicable.
    for (int i = 0; i < 6; i++)
    {
        int itemIdx = (int)vertices.size() - (i + 1);
        vertices[itemIdx] = rotateScaleTransform * (vertices[itemIdx] + translation);
        normals[itemIdx] = rotateScaleTransform * normals[itemIdx];
    }
}
