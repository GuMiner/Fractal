#pragma once
#include <glm\mat4x4.hpp>
#include <functional>
#include <map>
#include <vector>
#include "IObject.h"

// Shows a menu of objects to load with a UI to load and unload them.
class ObjectLoader
{
    int selectedObject;
    std::vector<IObject*> objects;

    std::vector<std::string> backingObjectNames;
    std::vector<const char*> objectNames;

    int selectedObjectToCreate;
    std::vector<const char*> objectCreationNames;
    std::map<std::string, std::function<IObject*()>> objectCreationMap;

    void UpdateObjectNames();
    void DisplayLoaderDialog();

public:
    ObjectLoader();
    void Update(float frameTime);
    void Render(glm::mat4& projectionMatrix);
};

