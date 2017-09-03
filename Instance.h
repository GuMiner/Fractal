#pragma once
#include <glm\vec4.hpp>
#include <glm\mat4x4.hpp>
#include <vector>

struct Instance
{
    // If true, this instance will be deleted. The Geometry will also be removed if this is the last active instance on the geometry.
    bool flaggedForDeletion;

    // The ID of the geometry this instance refers to.
    long long geometryId;

    // The characteristics of this instance.
    glm::mat4 transformation;
    glm::vec4 diffuseAlbedo;
    glm::vec4 specularAlbedo;
};