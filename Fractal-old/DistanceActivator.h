#pragma once
#include "IObjectActivator.h"

// Defines an activator that shows nearby objects and hides far away objects.
class DistanceActivator : public IObjectActivator
{
    float distanceThresholdSqd;

public:
    DistanceActivator(float distanceThreshold);

    // Te
    virtual bool IsObjectVisible(glm::vec3 playerPosition, BaseObject* baseObject) override;
    virtual std::vector<std::pair<GeometryGenerationData*, std::vector<Instance*>>> GetNewGeometryAndInstances(glm::vec3 playerPosition, BaseObject * baseObject, std::unordered_map<long long, Geometry*>* existingGeometries) override;
};

