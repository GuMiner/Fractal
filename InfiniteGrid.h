#pragma once
#include "BaseObjectType.h"

class InfiniteGrid : public BaseObjectType
{
public:
    InfiniteGrid();
    ~InfiniteGrid();

    virtual std::string GetName()  override;
};

