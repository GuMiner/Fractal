#pragma once
#include <glm\mat4x4.hpp>
#include <vector>
#include "IObject.h"

// Shows a menu of objects to load with a UI to load and unload them.
class ObjectLoader
{
    std::vector<IObject*> objects;

    void DisplayLoaderDialog();

public:
    ObjectLoader();
    void Update(float frameTime);
    void Render(glm::mat4& projectionMatrix);
};

