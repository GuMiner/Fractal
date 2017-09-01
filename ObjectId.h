#pragma once
#include <glm\vec3.hpp>

// Defines an object identifier for standardization
class ObjectId
{
public:
    long long objectId;

    ObjectId(const glm::ivec3 objectId);
    ObjectId(const int objectId);
};

