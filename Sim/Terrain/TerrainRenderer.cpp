#include <iostream>
#include <glm\gtc\matrix_transform.hpp>
#include <gl/glew.h>
#include "../Telemetry/Logger.h"
#include "../Time.h"
#include "../Diagnostic/DebugViewer.h"
#include "TerrainRenderer.h"

TerrainRenderer::TerrainRenderer(): uniforms() {

}

bool TerrainRenderer::Init(ShaderFactory* shaderFactory) {
    if (!shaderFactory->CreateShaderProgramWithGeometryShader("Config/Shaders/terrain", &modelProgram))
    {
        Logger::LogError("Failed to load the terrain rendering shader; cannot continue.");
        return false;
    }

    uniforms.SetProgramId(modelProgram);

    return true;
}

void TerrainRenderer::StartRender(Camera* camera, float maxHeight) {
    glUseProgram(modelProgram);

    glActiveTexture(GL_TEXTURE0);

    GLint normalsTexture = glGetUniformLocation(modelProgram, "normals");
    glUniform1i(normalsTexture, 0);

    // Colorations
    uniforms.SetFloat("maxHeight", maxHeight);

    uniforms.SetVec3("WaterColor", DebugViewer::Global->WaterColor);
    uniforms.SetFloat("WaterGrassCutoff", DebugViewer::Global->WaterGrassCutoff);
    uniforms.SetVec3("GrassColor", DebugViewer::Global->GrassColor);
    uniforms.SetFloat("GrassForestCutoff", DebugViewer::Global->GrassForestCutoff);
    uniforms.SetVec3("ForestColor", DebugViewer::Global->ForestColor);
    uniforms.SetFloat("ForestTundraCutoff", DebugViewer::Global->ForestTundraCutoff);
    uniforms.SetVec3("TundraColor", DebugViewer::Global->TundraColor);
    uniforms.SetFloat("TundraRockCutoff", DebugViewer::Global->TundraRockCutoff);
    uniforms.SetVec3("RockColor", DebugViewer::Global->RockColor);
    uniforms.SetFloat("RockSnowCutoff", DebugViewer::Global->RockSnowCutoff);
    uniforms.SetVec3("SnowColor", DebugViewer::Global->SnowColor);

    // Projection 
    modelShaderPos = glGetUniformLocation(modelProgram, "model");
    normalMatrixShaderPos = glGetUniformLocation(modelProgram, "normalMatrix");

    uniforms.SetMat4("view", camera->View);
    uniforms.SetMat4("perspective", camera->Perspective);
    
    // Lighting
    uniforms.SetMaterialLighting(
        glm::vec3(0.4f, 0.4f, 0.4f), // ambient
        glm::vec3(0.6f, 0.6f, 0.6f), // diffuse
        glm::vec3(0.5f, 0.5f, 0.5f), // specular
        4.0f); // specular strength

    auto lightDirection = Time::GlobalTime->SunDirection();
    auto lightIntensity = Time::GlobalTime->SunIntensity() * 0.9f;

    auto viewLightDirection = camera->View * glm::vec4(lightDirection, 1.0f);
    uniforms.SetDirectionalLighting(
        glm::normalize(glm::vec3(viewLightDirection)),
        glm::vec3(lightIntensity / 2),
        glm::vec3(lightIntensity)
    );

    auto pointOnePosition = camera->Position() + (10.0f * camera->Forwards());
    auto pointOneViewPos = camera->View * glm::vec4(pointOnePosition, 1.0f);
    uniforms.SetPointOneLighting(
        glm::vec3(pointOneViewPos),
        glm::vec3(0.2f, 0.2f, 0.2f),
        glm::vec3(0.9f, 0.8f, 0.4f)
    );
}

void TerrainRenderer::StopRender() {
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
    glUseProgram(0);
}

void TerrainRenderer::Render(Camera* camera, glm::mat4 position, TerrainModel* model) {
    glBindVertexArray(model->GetVao());

    glBindTexture(GL_TEXTURE_2D, model->GetNormalTexture());

    glUniformMatrix4fv(modelShaderPos, 1, GL_FALSE, &position[0][0]);

    // TODO both for this and the grid, this only works if all *lights* (like the sun) are also moved around.
    glm::mat4 normalMatrixComputed = glm::transpose(glm::inverse(camera->View * position));
    glUniformMatrix4fv(normalMatrixShaderPos, 1, GL_FALSE, &normalMatrixComputed[0][0]);

    glDrawElements(GL_TRIANGLES, 3 * model->GetFaceCount(), GL_UNSIGNED_INT, 0);
}