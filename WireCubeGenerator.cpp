#define _USE_MATH_DEFINES
#include <cmath>
#include <limits>
#include <vector>
#include <glm\vec2.hpp>
#include <glm\vec3.hpp>
#include <glm\gtc\random.hpp>
#include <glm\gtx\transform.hpp>
#include "logging\Logger.h"
#include "BoundedTriangleUvMapper.h"
#include "WireCubeGenerator.h"

WireCubeGenerator::WireCubeGenerator()
{
}

void WireCubeGenerator::AddPlane(std::vector<glm::vec3>& vertices, std::vector<glm::vec3>& normals, glm::mat3 planeRotation)
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
        vertices[itemIdx] = planeRotation * vertices[itemIdx];
        normals[itemIdx] = planeRotation * normals[itemIdx];
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
        texture[i * 4 + 0] = (unsigned char)((i) % 256);
        texture[i * 4 + 1] = (unsigned char)((i * i) % 256);
        texture[i * 4 + 2] = (unsigned char)((i * i * i) % 256);
        texture[i * 4 + 3] = 255;
    }

    // We're unfolding the cube manually, but we really want to do this automatically.
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uvs;
    
    float ninetyDegrees = M_PI_2;
    AddPlane(vertices, normals, glm::mat3());
    AddPlane(vertices, normals, glm::rotate(ninetyDegrees, glm::vec3(0, 1, 0)));
    AddPlane(vertices, normals, glm::rotate(ninetyDegrees * 2, glm::vec3(0, 1, 0)));
    AddPlane(vertices, normals, glm::rotate(ninetyDegrees * 3, glm::vec3(0, 1, 0)));

    AddPlane(vertices, normals, glm::rotate(ninetyDegrees, glm::vec3(0, 0, 1)) * glm::rotate(ninetyDegrees, glm::vec3(0, 1, 0)));
    AddPlane(vertices, normals, glm::rotate(-ninetyDegrees, glm::vec3(0, 0, 1)) * glm::rotate(ninetyDegrees, glm::vec3(0, 1, 0)));

    // Simulate cube construction to verify our uv mapper, as we want to verify this works before creating test geometry.
    CoreGeometry geo;
    geo.SetGeometryData(vertices, normals);

    BoundedTriangleUvMapper uvMapper;
    float triangleScaleDownAmount;
    
    uvs = uvMapper.MapToTexturePlane(geo, &triangleScaleDownAmount);

    geometryToGenerate->SetGeometryData(width, height, texture, vertices, normals, uvs);
}
