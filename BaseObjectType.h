#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <glm\vec3.hpp>
#include "BaseObject.h"
#include "IGeometryGenerator.h"
#include "IObjectActivator.h"
#include "IPerformanceProfiler.h"
#include "IStandardRenderer.h"
#include "Geometry.h"
#include "GeometryGenerationScheduler.h"

// Defines a base object type for procedural object generation and rendering. See below for details
class BaseObjectType
{
    GeometryGenerationScheduler* scheduler;
    IObjectActivator* objectActivator;
    IGeometryGenerator* geometryGenerator;

    // The objects stored within this base object type.
    std::vector<BaseObject*> objects;

    // Mapping geometry IDs to geometries.
    std::unordered_map<long long, Geometry*> geometries;

    // A reverse mapping, mapping geometry types to instances. Useful for high-speed rendering.
    std::unordered_map<Geometry*, std::vector<Instance*>> geometryInstances;

protected:
    BaseObjectType(IObjectActivator* objectActivator, IGeometryGenerator* geometryGenerator, GeometryGenerationScheduler* scheduler);

    // Performs per-instance updates. By default, performs nothing.
    virtual void Update(float gameTime, float frameTime, Instance* instance);
    
public:
    virtual ~BaseObjectType();
    virtual std::string GetName() = 0;

    void Update(float gameTime, float frameTime);
    void Render(IStandardRenderer* standardRenderer, IPerformanceProfiler* profiler);
};

// So how does procedurally-generated rate-limited polygon generation and rendering work?
