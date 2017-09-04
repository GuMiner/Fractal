#pragma once
#include "BaseObjectType.h"

class InfiniteGrid : public BaseObjectType
{
public:
    InfiniteGrid(IObjectActivator* objectActivator, IGeometryGenerator* geometryGenerator, GeometryGenerationScheduler* scheduler);
    ~InfiniteGrid();

    virtual std::string GetName()  override;
};

