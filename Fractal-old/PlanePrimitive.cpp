#define _USE_MATH_DEFINES
#include <cmath>
#include <glm\gtx\transform.hpp>
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

void PlanePrimitive::AddPlaneFan(std::vector<glm::vec3>& vertices, std::vector<glm::vec3>& normals, int segments, glm::mat4 transform)
{
    glm::vec3 normal = glm::vec3(0, 1, 0);

    float angleStep = (float)(2.0f * M_PI) / (float)segments;
    glm::vec3 currentVector = glm::vec3(0, 0, -1);
    currentVector = glm::mat3(glm::rotate(-angleStep / 2.0f, normal)) * currentVector;

    glm::mat3 angularRotationMatrix = glm::rotate(-angleStep, glm::vec3(0, 1, 0));
    for (int i = 0; i < segments; i++)
    {
        vertices.push_back(glm::vec3(0, 0, 0));
        normals.push_back(normal);

        vertices.push_back(currentVector);
        normals.push_back(normal);
        
        currentVector = angularRotationMatrix * currentVector;
        vertices.push_back(currentVector);
        normals.push_back(normal);
    }

    // Loopback six items and rotate as applicable.
    for (int i = 0; i < segments * 3; i++)
    {
        int itemIdx = (int)vertices.size() - (i + 1);

        vertices[itemIdx] = glm::vec3(transform * glm::vec4(vertices[itemIdx].x, vertices[itemIdx].y, vertices[itemIdx].z, 1.0f));
        normals[itemIdx] = glm::mat3(transform) * normals[itemIdx];
    }
}
