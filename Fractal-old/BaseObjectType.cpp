#include "BaseObjectType.h"

BaseObjectType::BaseObjectType(IObjectActivator* objectActivator, IGeometryGenerator* geometryGenerator, GeometryGenerationScheduler* scheduler)
    : objectActivator(objectActivator), geometryGenerator(geometryGenerator), scheduler(scheduler)
{
}

BaseObjectType::~BaseObjectType()
{
}

void BaseObjectType::Update(float gameTime, float frameTime, Instance* instance)
{
    // Per-instance updates is left for base classes to perform, if desired.
}

void BaseObjectType::AddObject(BaseObject* object)
{
    objects.push_back(object);
}

void BaseObjectType::Update(glm::vec3 playerPosition, float gameTime, float frameTime)
{
    // See what stuff should exist and queue geometry for generation if necessary
    for (BaseObject* object : objects)
    {
        // TODO: Most of our objects (single-object mode) aren't going to be near-visible. We need to be more efficient than this.
        if (objectActivator->IsObjectVisible(playerPosition, object))
        {
            std::vector<std::pair<GeometryGenerationData*, std::vector<Instance*>>> newGeometryAndInstances =
                objectActivator->GetNewGeometryAndInstances(playerPosition, object, &geometries);

            for (std::pair<GeometryGenerationData*, std::vector<Instance*>> instances : newGeometryAndInstances)
            {
                Geometry* geo = new Geometry();
                geo->geometryId = instances.first->geometryId;

                geometryInstances[geo] = std::vector<Instance*>();
                for (Instance* instance : instances.second)
                {
                    instance->geometryId = geo->geometryId;
                    instance->flaggedForDeletion = false;

                    geometryInstances[geo].push_back(instance);
                    object->instances.push_back(instance);
                }

                geometries[geo->geometryId] = geo;
                scheduler->AddGeometryGenerationTask(geometryGenerator, instances.first, geo);
            }
        }
        else
        {
            // If the object isn't visible, it shouldn't have any instances. Flag all to be deleted
            for (Instance* instance : object->instances)
            {
                instance->flaggedForDeletion = true;
            }
        }

        for (int i = (int)object->instances.size() - 1; i >= 0; i--)
        {
            Instance* instance = object->instances[i];
            if (instance->flaggedForDeletion)
            {
                // Remove the instance from associated geometry.
                Geometry* associatedGeo = geometries[instance->geometryId];
                for (int j = (int)geometryInstances[associatedGeo].size(); j >= 0; j--)
                {
                    if (geometryInstances[associatedGeo][j] == instance)
                    {
                        geometryInstances[associatedGeo].erase(geometryInstances[associatedGeo].begin() + j);
                        break;
                    }
                }

                // Remove the instance itself
                object->instances.erase(object->instances.begin() + i);
                delete instance;

                // Remove the geometry if it is no longer referenced by any instances
                if (geometryInstances[associatedGeo].size() == 0)
                {
                    geometries.erase(associatedGeo->geometryId);
                    geometryInstances.erase(associatedGeo);
                    delete associatedGeo;
                }
            }
        }
    }

    // Unlike 'World', we want to update everything near us that has generated geometry
    for (std::pair<const Geometry*, std::vector<Instance*>> geometrySet : geometryInstances)
    {
        for (Instance* objectInstance : geometrySet.second)
        {
            Update(gameTime, frameTime, objectInstance);
        }
    }
}

void BaseObjectType::Render(IStandardRenderer* standardRenderer, IPerformanceProfiler* profiler)
{
    // Perform the transfer pass
    for (std::pair<Geometry*, std::vector<Instance*>> geometrySet : geometryInstances)
    {
        if (geometrySet.first->CanSendToGpu())
        {
            int activeTextureOffset = 0;// TODO Get from caps somewhere.
            geometrySet.first->SendToGpu(activeTextureOffset, profiler);
        }
    }

    // Perform the render pass
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
