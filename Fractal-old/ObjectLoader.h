#pragma once
#include <glm\vec3.hpp>
#include <glm\mat4x4.hpp>
#include <functional>
#include <map>
#include <vector>
#include "BaseObjectType.h"

// Shows a menu of objects to load with a UI to load and unload them.
class ObjectLoader
{
    int lastSelectedObject;
    glm::vec3 selectedObjectPosition;

    int selectedObject;
    std::vector<BaseObjectType*> objects;

    std::vector<std::string> backingObjectNames;
    std::vector<const char*> objectNames;

    int selectedObjectToCreate;
    std::vector<const char*> objectCreationNames;
    std::map<std::string, std::function<BaseObjectType*()>> objectCreationMap;

    void UpdateObjectNames();

public:
    ObjectLoader(GeometryGenerationScheduler* scheduler);
    void Render();
};

