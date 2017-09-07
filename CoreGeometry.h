#pragma once
#include <vector>
#include <glm\common.hpp>
#include <glm\vec3.hpp>
#include "vertex\NormalVbo.hpp"
#include "vertex\PositionVbo.hpp"

class CoreGeometry
{
    NormalVbo normalVbo;
    PositionVbo positionVbo;
public:
    CoreGeometry();
    
    void SetGeometryData(std::vector<glm::vec3>& vertices, std::vector<glm::vec3>& normals);
    size_t GetMaxVertexCount() const;
    void NormalizeVertexCount(int maxVertexCount);

    void SetupOpenGlBuffers();
    void TransferToOpenGl();
    void Deinitialize();
};

