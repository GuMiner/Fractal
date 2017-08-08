#pragma once
#include <glm\vec4.hpp>
#include <glm\mat4x4.hpp>

struct Instance
{
    int objectId;
    glm::mat4 transformation;
    glm::vec4 diffuseAlbedo;
    glm::vec4 specularAlbedo;
};