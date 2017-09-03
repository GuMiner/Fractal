#pragma once
#include <glm\vec3.hpp>
#include "Geometry.h"
#include "IGeometryGenerationData.h"

// Defines how to populate geometry
class IGeometryGenerator
{
public:
    virtual void GenerateGeometry(IGeometryGenerationData* geometryGenerationData, Geometry* geometryToGenerate) = 0;
};

