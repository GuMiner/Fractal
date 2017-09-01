#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <glm\vec3.hpp>
#include "IObject.h"
#include "IStandardRenderer.h"
#include "IPerformanceProfiler.h"
#include "Lod.h"
#include "Geometry.h"

typedef int LodX;
typedef int LodY;
typedef int LodZ;

typedef int GeomIdX;
typedef int GeomIdY;
typedef int GeomIdZ;

class BaseObject
{
    // The mapping of LOD -> IDs of instances of geometry that must be displayed
    std::unordered_map<LodX, std::unordered_map<LodY, std::unordered_map<LodZ, std::vector<glm::ivec3>>>> lodToGeometryInstances;

    // Mapping of object instance IDs -> object instances. Each geometry instance can be used by many LOD instances
    std::unordered_map<GeomIdX, std::unordered_map<GeomIdY, std::unordered_map<GeomIdZ, Instance*>>> geometryInstanceIdToGeometryInstance;

    // Mapping geometry IDs to geometries.
    std::unordered_map<unsigned int, Geometry*> geometries;

    // A reverse mapping, mapping geometry types to instances. Useful for high-speed rendering.
    std::unordered_map<Geometry*, std::vector<Instance*>> geometryInstances;

    std::string name;
protected:
    BaseObject(std::string name);
    ~BaseObject();

    std::string GetName();
    // virtual void Update(float frameTime) override;

    void Render(IStandardRenderer* standardRenderer, IPerformanceProfiler* profiler)
    {
        for (std::pair<const Geometry*, std::vector<Instance*>> geometrySet : geometryInstances)
        {
            // Stop rendering if we hit our rendering limit
            if (profiler->HasFrameRenderHitLimit())
            {
                break;
            }

            geometrySet.first->Render(geometrySet.second, standardRenderer, profiler);
        }
    }
};

