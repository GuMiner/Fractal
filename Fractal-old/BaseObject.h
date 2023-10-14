#pragma once
#include <glm\vec3.hpp>
#include <vector>
#include "Instance.h"

// Defines the minimal subset of data ann object needs 
class BaseObject
{
public:
    glm::vec3 objectPosition;
    // TODO need rotation and other core stuff to be here.

    // The physical geometric instances that make up this object.
    std::vector<Instance*> instances;

    BaseObject();
    ~BaseObject();
};

