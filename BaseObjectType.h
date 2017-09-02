#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <glm\vec3.hpp>
#include "IObject.h"
#include "IStandardRenderer.h"
#include "IPerformanceProfiler.h"
#include "Geometry.h"

typedef int LodX;
typedef int LodY;
typedef int LodZ;

typedef int GeomIdX;
typedef int GeomIdY;
typedef int GeomIdZ;
typedef int GeomIdW;

// Defines a base object type for procedural object generation and rendering. See below for details
class BaseObjectType
{
    // The mapping of LOD -> IDs of instances of geometry that must be displayed
    std::unordered_map<LodX, std::unordered_map<LodY, std::unordered_map<LodZ, std::vector<glm::ivec4>>>> lodToGeometryInstances;

    // Mapping of object instance IDs -> object instances. Each geometry instance can be used by many LOD instances
    std::unordered_map<GeomIdX, std::unordered_map<GeomIdY, std::unordered_map<GeomIdZ, std::unordered_map<GeomIdW, Instance*>>>> geometryInstanceIdToGeometryInstance;

    // Mapping geometry IDs to geometries.
    std::unordered_map<unsigned int, Geometry*> geometries;

    // A reverse mapping, mapping geometry types to instances. Useful for high-speed rendering.
    std::unordered_map<Geometry*, std::vector<Instance*>> geometryInstances;

protected:
    BaseObjectType();

    // Performs per-instance updates. By default, performs nothing.
    virtual void Update(float gameTime, float frameTime, Instance* instance);
    
public:
    virtual ~BaseObjectType();
    virtual std::string GetName() = 0;

    void Update(float gameTime, float frameTime);
    void Render(IStandardRenderer* standardRenderer, IPerformanceProfiler* profiler);
};

// So how does procedurally-generated rate-limited polygon generation and rendering work?
