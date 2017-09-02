#include "BaseObjectType.h"

BaseObjectType::BaseObjectType()
{
}

BaseObjectType::~BaseObjectType()
{
}

void BaseObjectType::Update(float gameTime, float frameTime, Instance* instance)
{
    // Per-instance updates is left for base classes to perform, if desired.
}

void BaseObjectType::Update(float gameTime, float frameTime)
{
    // Unlike 'World', we want to update everything near us.
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
