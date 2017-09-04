#pragma once
#include <glm\vec3.hpp>
#include "Geometry.h"
#include "GeometryGenerationData.h"

// Defines how to populate geometry
class IGeometryGenerator
{
public:
    virtual void GenerateGeometry(GeometryGenerationData* geometryGenerationData, Geometry* geometryToGenerate) = 0;
};

