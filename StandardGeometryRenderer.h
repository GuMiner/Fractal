#pragma once
#include "shaders\ShaderFactory.h"
#include "IStandardRenderer.h"

class StandardGeometryRenderer : public IStandardRenderer
{
    GLuint programId;

    GLuint projectionMatrixLocation;
    GLuint modelMatrixLocation;

    GLuint diffuseAlbedoLocation;
    GLuint specularAlbedoLocation;

    GLuint geometryTextureLocation;

    int currentGeometryVertexCount;

public:
    StandardGeometryRenderer();
    bool LoadProgram(ShaderFactory* shaderFactory);

    // Inherited via IStandardRenderer
    virtual void StartRendering(const glm::mat4& projectionMatrix) override;
    virtual void SwapToGeometry(GLuint vao, int activeTextureOffset, GLuint textureId, int vertexCount) override;
    virtual void RenderInstance(const Instance* instance) override;
};

