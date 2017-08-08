#pragma once
#include <string>
#include <map>
#include "IObject.h"
#include "Lod.h"
#include "Geometry.h"

class BaseObject
{
    std::map<Lod, Geometry> generatedLods;

    std::string name;
protected:
    BaseObject(std::string name);
    ~BaseObject();

    std::string GetName();
    // virtual void Update(float frameTime) override;
    // virtual void Render(glm::mat4 & projectionMatrix) override;
};

