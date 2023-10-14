#include "logging\Logger.h"
#include "StandardGeometryRenderer.h"

StandardGeometryRenderer::StandardGeometryRenderer()
{
}

bool StandardGeometryRenderer::LoadProgram(ShaderFactory* shaderFactory)
{
    Logger::Log("Loading the standard geometry renderer...");
    if (!shaderFactory->CreateShaderProgram("standardGeometry", &programId))
    {
        Logger::LogError("Failed to load the standard geometry renderer; cannot continue.");
        return false;
    }

    projectionMatrixLocation = glGetUniformLocation(programId, "projectionMatrix");
    modelMatrixLocation = glGetUniformLocation(programId, "modelMatrix");

    diffuseAlbedoLocation = glGetUniformLocation(programId, "diffuseAlbedo");
    specularAlbedoLocation = glGetUniformLocation(programId, "specularAlbedo");

    geometryTextureLocation = glGetUniformLocation(programId, "geometryTexture");

    return true;
}

void StandardGeometryRenderer::StartRendering(const glm::mat4& projectionMatrix)
{
    glUseProgram(programId);
    glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
}

void StandardGeometryRenderer::SwapToGeometry(GLuint vao, int activeTextureOffset, GLuint textureId, int vertexCount)
{
    currentGeometryVertexCount = vertexCount;
    glBindVertexArray(vao);

    glActiveTexture(GL_TEXTURE0 + activeTextureOffset);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glUniform1i(geometryTextureLocation, activeTextureOffset);
}

void StandardGeometryRenderer::RenderInstance(const Instance* instance)
{
    glUniform4fv(diffuseAlbedoLocation, 1, &instance->diffuseAlbedo[0]);
    glUniform4fv(specularAlbedoLocation, 1, &instance->specularAlbedo[0]);
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &instance->transformation[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, currentGeometryVertexCount);
}
