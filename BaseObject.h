#pragma once
#include <vector>
#include "Instance.h"

// Defines the minimal subset of data ann object needs 
class BaseObject
{
public:
    // The physical geometric instances that make up this object.
    std::vector<Instance*> instances;

    BaseObject();
    ~BaseObject();
};

