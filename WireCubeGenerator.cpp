#include <limits>
#include <vector>
#include <glm\vec2.hpp>
#include <glm\vec3.hpp>
#include <glm\gtc\random.hpp>
#include <glm\gtx\transform.hpp>
#include "logging\Logger.h"
#include "BoundedTriangleUvMapper.h"
#include "CubeShape.h"
#include "CylinderShape.h" // TODO remove after testing
#include "WireCubeGenerator.h"

WireCubeGenerator::WireCubeGenerator()
{
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

    // TODO: Make this a CUBE again once testing completes.
    // CubeShape cube(1);
    CylinderShape cube(8);
    CoreGeometry geo = cube.Generate();

    BoundedTriangleUvMapper uvMapper;
    float triangleScaleDownAmount;
    
    std::vector<glm::vec2> uvs = uvMapper.MapToTexturePlane(geo, &triangleScaleDownAmount);
    geometryToGenerate->SetGeometryData(width, height, texture, geo, uvs);
}
