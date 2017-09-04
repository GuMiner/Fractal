#define _USE_MATH_DEFINES
#include <cmath>
#include <limits>
#include <vector>
#include <glm\vec2.hpp>
#include <glm\vec3.hpp>
#include <glm\gtc\random.hpp>
#include <glm\gtx\transform.hpp>
#include "logging\Logger.h"
#include "WireCubeGenerator.h"

WireCubeGenerator::WireCubeGenerator()
{
}

void WireCubeGenerator::AddPlane(std::vector<glm::vec3>& vertices, std::vector<glm::vec3>& normals, std::vector<glm::vec2>& uvs,
    glm::mat3 planeRotation, glm::vec2 uvTranslation)
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

    // Our UVs start as a 1/12th segment in this location:
    // - - - -
    // * - - -
    // - - - -
    float oneThirds = 1.0f / 3.0f;
    float twoThirds = 2.0f / 3.0f;
    uvs.push_back(glm::vec2(0.0f, twoThirds));
    uvs.push_back(glm::vec2(0.25f, twoThirds));
    uvs.push_back(glm::vec2(0.25f, oneThirds));

    uvs.push_back(glm::vec2(0.0f, twoThirds));
    uvs.push_back(glm::vec2(0.25f, oneThirds));
    uvs.push_back(glm::vec2(0.0f, oneThirds));

    // Lookback six items and rotate / move UVs as applicable.
    for (int i = 0; i < 6; i++)
    {
        int itemIdx = (int)vertices.size() - (i + 1);
        vertices[itemIdx] = planeRotation * vertices[itemIdx];
        normals[itemIdx] = planeRotation * normals[itemIdx];
        uvs[itemIdx] += uvTranslation;
    }
}

void WireCubeGenerator::GenerateGeometry(GeometryGenerationData* geometryGenerationData, Geometry* geometryToGenerate)
{
    // TODO: Make this dependent on LOD, gen data, etci
    Logger::Log("Wire Cube LOD: ", geometryGenerationData->lod, " Geo ID: ", geometryGenerationData->lod);
    Logger::Log("Numeric limits test: ", (int)std::numeric_limits<unsigned char>::min(), " ", (int)std::numeric_limits<unsigned char>::max());
    // Set the texture to pure randomness (easy to see)
    int width = 512;
    int height = 512;
    std::vector<unsigned char> texture;
    texture.resize(width * height * 4);
    for (int i = 0; i < width * height; i++)
    {
        texture[i * 4 + 0] = (unsigned char)(rand() % 256);
        texture[i * 4 + 1] = (unsigned char)(rand() % 256);
        texture[i * 4 + 2] = (unsigned char)(rand() % 256);

        // Random transparency. Should be rather interesting to observe.
        texture[i * 4 + 3] = (unsigned char)(rand() % 256);
    }

    // We're unfolding the cube manually, but we really want to do this automatically.
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uvs;
    
    float ninetyDegrees = M_PI_2;
    AddPlane(vertices, normals, uvs, glm::mat3(), glm::vec2(0, 0));
    AddPlane(vertices, normals, uvs, glm::rotate(ninetyDegrees, glm::vec3(0, 1, 0)), glm::vec2(0.25, 0));
    AddPlane(vertices, normals, uvs, glm::rotate(ninetyDegrees * 2, glm::vec3(0, 1, 0)), glm::vec2(0.50, 0));
    AddPlane(vertices, normals, uvs, glm::rotate(ninetyDegrees * 3, glm::vec3(0, 1, 0)), glm::vec2(0.75, 0));

    AddPlane(vertices, normals, uvs, 
        glm::rotate(ninetyDegrees, glm::vec3(0, 0, 1)) * glm::rotate(ninetyDegrees, glm::vec3(0, 1, 0)), glm::vec2(0.25, 0.33));
    AddPlane(vertices, normals, uvs,
        glm::rotate(-ninetyDegrees, glm::vec3(0, 0, 1)) * glm::rotate(ninetyDegrees, glm::vec3(0, 1, 0)), glm::vec2(0.25, -0.33));

    geometryToGenerate->SetGeometryData(width, height, texture, vertices, normals, uvs);
}
