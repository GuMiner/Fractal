#pragma once
#include <vector>
#include <glm\vec2.hpp>
#include <glm\vec3.hpp>
#include <glm\mat3x3.hpp>
#include "IGeometryGenerator.h"

// Creates geometry for a wireframe cube.
class WireCubeGenerator : public IGeometryGenerator
{
    // Adds a plane in the cube generator.
    void AddPlane(std::vector<glm::vec3>& vertices, std::vector<glm::vec3>& normals, std::vector<glm::vec2>& uvs,
        glm::mat3 planeRotation, glm::vec2 uvTranslation);

public:
    WireCubeGenerator();

    // Inherited via IGeometryGenerator
    virtual void GenerateGeometry(GeometryGenerationData* geometryGenerationData, Geometry* geometryToGenerate) override;
};

