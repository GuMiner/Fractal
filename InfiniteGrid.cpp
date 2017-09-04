#include "InfiniteGrid.h"

InfiniteGrid::InfiniteGrid(IObjectActivator* objectActivator, IGeometryGenerator* geometryGenerator, GeometryGenerationScheduler* scheduler)
    : BaseObjectType(objectActivator, geometryGenerator, scheduler)
{
}

InfiniteGrid::~InfiniteGrid()
{
}

std::string InfiniteGrid::GetName()
{
    return "Infinite Grid";
}
