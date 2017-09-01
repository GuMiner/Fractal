#pragma once
#include <glm\vec4.hpp>
#include <glm\mat4x4.hpp>
#include <vector>

struct Instance
{
    // The numbers of LODs that are referencing this instance.
    unsigned int LodReferences;

    // The ID of the geometry this instance refers to.
    unsigned int GeometryId;

    // The characteristics of this instance.
    glm::mat4 transformation;
    glm::vec4 diffuseAlbedo;
    glm::vec4 specularAlbedo;
};