#include <glm\gtc\matrix_transform.hpp>
#include <gl/glew.h>
#include "../Telemetry/Logger.h"
#include "../Time.h"
#include "TerrainRenderer.h"

TerrainRenderer::TerrainRenderer() {

}

bool TerrainRenderer::Init(ShaderFactory* shaderFactory) {
    if (!shaderFactory->CreateShaderProgramWithGeometryShader("Config/Shaders/terrain", &modelProgram))
    {
        Logger::LogError("Failed to load the terrain rendering shader; cannot continue.");
        return false;
    }

    return true;
}

void TerrainRenderer::StartRender(Camera* camera) {
    glUseProgram(modelProgram);

    glActiveTexture(GL_TEXTURE0);

    GLint normalsTexture = glGetUniformLocation(modelProgram, "normals");
    glUniform1i(normalsTexture, 0);

    // Projection 
    modelShaderPos = glGetUniformLocation(modelProgram, "model");
    normalMatrixShaderPos = glGetUniformLocation(modelProgram, "normalMatrix");

    GLint view = glGetUniformLocation(modelProgram, "view");
    glUniformMatrix4fv(view, 1, GL_FALSE, &camera->View[0][0]);

    GLint perspective = glGetUniformLocation(modelProgram, "perspective");
    glUniformMatrix4fv(perspective, 1, GL_FALSE, &camera->Perspective[0][0]);

    // Lighting
    GLint ambient = glGetUniformLocation(modelProgram, "ambient");
    glUniform3f(ambient, 0.501f, 0.501f, 0.501f);

    GLint diffuse = glGetUniformLocation(modelProgram, "diffuse");
    glUniform3f(diffuse, 0.51f, 0.51f, 0.51f);

    GLint specularColor = glGetUniformLocation(modelProgram, "specularColor");
    glUniform3f(specularColor, 0.51f, 0.51f, 0.51f);

    GLint specular = glGetUniformLocation(modelProgram, "specular");
    glUniform1f(specular, 4.0f);

    auto lightDirection = Time::GlobalTime->SunDirection();
    auto lightIntensity = Time::GlobalTime->SunIntensity() * 0.9f;

    GLint dLightDirection = glGetUniformLocation(modelProgram, "dLightDirection");
    glUniform3f(dLightDirection, lightDirection.x, lightDirection.y, lightDirection.z);

    GLint dLightAmbient = glGetUniformLocation(modelProgram, "dLightAmbient");
    glUniform3f(dLightAmbient, lightIntensity / 2, lightIntensity / 2, lightIntensity / 2);

    GLint dLightDiffuse = glGetUniformLocation(modelProgram, "dLightDiffuse");
    glUniform3f(dLightDiffuse, lightIntensity, lightIntensity, lightIntensity);

    GLint pLightPosition = glGetUniformLocation(modelProgram, "pLightPosition");
    auto lightPosition = glm::vec3(5.0f, 5.0f, -5.0);
    glUniform3f(pLightPosition, lightPosition.x, lightPosition.y, lightPosition.z);

    GLint pLightAmbient = glGetUniformLocation(modelProgram, "pLightAmbient");
    glUniform3f(pLightAmbient, 0.002f, 0.002f, 0.002f);

    GLint pLightDiffuse = glGetUniformLocation(modelProgram, "pLightDiffuse");
    glUniform3f(pLightDiffuse, 0.008f, 0.008f, 0.008f);
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

    glm::mat4 normalMatrixComputed = glm::transpose(glm::inverse(camera->View * position));
    glUniformMatrix4fv(normalMatrixShaderPos, 1, GL_FALSE, &normalMatrixComputed[0][0]);

    glDrawElements(GL_TRIANGLES, 3 * model->GetFaceCount(), GL_UNSIGNED_INT, 0);


}