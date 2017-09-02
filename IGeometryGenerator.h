#pragma once
#include <glm\vec3.hpp>
#include "Geometry.h"

// Defines how to populate geometry
class IGeometryGenerator
{
public:
    virtual void GenerateGeometry(glm::ivec3 geometryId, Geometry* geometryToGenerate) = 0;
};

