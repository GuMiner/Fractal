#pragma once
#include <unordered_map>
#include <vector>
#include <glm\vec3.hpp>
#include "BaseObject.h"
#include "GeometryGenerationData.h"
#include "Geometry.h"

// Activates an object, forming geometry IDs necessary for it to render
//
// For single-type objects (one object corresponds to a piece of geometry), the activator
// really only needs to return a single number corresponding to the LOD we need the object to render it.
// Because we run this once per object, we'll get the appropriate geometry for all object instances.

// For multi-type objects (one object corresponds to multiple pieces of geometry), the activator
//  will also upate the base object (as necessary) 

class IObjectActivator
{
public:
    // Runs a quick check to see if the object is visible or not before further processing
    virtual bool IsObjectVisible(glm::vec3 playerPosition, BaseObject* baseObject) = 0;

    // Gets new geometry and instances on the base object. Also flags instances on the main object for deletion.
    virtual std::vector<std::pair<GeometryGenerationData*, std::vector<Instance*>>> GetNewGeometryAndInstances(glm::vec3 playerPosition, BaseObject* baseObject, std::unordered_map<long long, Geometry*>* existingGeometreis) = 0;
};

