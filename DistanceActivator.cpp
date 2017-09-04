#include <glm\gtx\norm.hpp>
#include <glm\gtx\transform.hpp>
#include "DistanceActivator.h"

DistanceActivator::DistanceActivator(float distanceThreshold)
    : distanceThresholdSqd(distanceThreshold * distanceThreshold)
{
}

bool DistanceActivator::IsObjectVisible(glm::vec3 playerPosition, BaseObject* baseObject)
{
    return glm::distance2(playerPosition, baseObject->objectPosition) < distanceThresholdSqd;
}

std::vector<std::pair<GeometryGenerationData*, std::vector<Instance*>>> DistanceActivator::GetNewGeometryAndInstances(
    glm::vec3 playerPosition, BaseObject* baseObject, std::unordered_map<long long, Geometry*>* existingGeometreis)
{
    std::vector<std::pair<GeometryGenerationData*, std::vector<Instance*>>> newGeometry;
    // TODO this only works for single-object mode.
    // Once I have this *working*, I'm going to completely revisit how this is designed.
    
    // If visible, we should have a single instance.
    if (baseObject->instances.size() == 0)
    {
        GeometryGenerationData* genData = new GeometryGenerationData();
        genData->generationData = nullptr; // TODO: This really should be defined by *child* classes, such as a wireframe distance activator.
        genData->lod = 1;
        genData->geometryId = 1; // Each activator defines what this means, in concert with the geometry generator. For a wireframe cube, we only have LOD == 1, with ID = this.
        // We may get rid of the geometry ID and just use LOD, and add a mapping of LOD - item.

        std::vector<Instance*> newInstances;
        Instance* instance = new Instance();

        // Set material to all a single type
        instance->diffuseAlbedo = glm::vec4(0.50f, 0.50f, 0.50f, 1.0f);
        instance->specularAlbedo = glm::vec4(0.50f, 0.50f, 0.50f, 1.0f);
        instance->transformation = glm::translate(baseObject->objectPosition); // TODO: Get rotation in here.
        instance->flaggedForDeletion = false;
        newInstances.push_back(instance);

        newGeometry.push_back(std::make_pair(genData, newInstances));
    }

    return newGeometry;
}
