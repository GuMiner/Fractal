#include <glm\gtc\matrix_transform.hpp>
#include <gl/glew.h>
#include "../Telemetry/Logger.h"
#include "TerrainRenderer.h"

TerrainRenderer::TerrainRenderer() {

}

bool TerrainRenderer::Init(ShaderFactory* shaderFactory) {
    if (!shaderFactory->CreateShaderProgram("Config/Shaders/model", &modelProgram))
    {
        Logger::LogError("Failed to load the model rendering shader; cannot continue.");
        return false;
    }

    return true;
}


void TerrainRenderer::Render(Camera* camera, float currentTime, TerrainModel* model, glm::mat4 position) {
    glUseProgram(modelProgram);
    glBindVertexArray(model->GetVao());

    // Projection 
    GLint modelPos = glGetUniformLocation(modelProgram, "model");
    glUniformMatrix4fv(modelPos, 1, GL_FALSE, &position[0][0]);

    GLint view = glGetUniformLocation(modelProgram, "view");
    glUniformMatrix4fv(view, 1, GL_FALSE, &camera->View[0][0]);

    GLint perspective = glGetUniformLocation(modelProgram, "perspective");
    glUniformMatrix4fv(perspective, 1, GL_FALSE, &camera->Perspective[0][0]);

    GLint normalMatrix = glGetUniformLocation(modelProgram, "normalMatrix");
    glm::mat4 normalMatrixComputed = glm::transpose(glm::inverse(camera->View * position));

    glUniformMatrix4fv(normalMatrix, 1, GL_FALSE, &normalMatrixComputed[0][0]);


    // Lighting
    GLint ambient = glGetUniformLocation(modelProgram, "ambient");
    glUniform3f(ambient, 0.51f, 0.51f, 0.51f);

    GLint diffuse = glGetUniformLocation(modelProgram, "diffuse");
    glUniform3f(diffuse, 0.51f, 0.51f, 0.51f);

    GLint specularColor = glGetUniformLocation(modelProgram, "specularColor");
    glUniform3f(specularColor, 0.51f, 0.51f, 0.51f);

    GLint specular = glGetUniformLocation(modelProgram, "specular");
    glUniform1f(specular, 4.0f);

    GLint dLightDirection = glGetUniformLocation(modelProgram, "dLightDirection");
    auto lightDirection = glm::normalize(glm::vec3(1.0f, 2.0f, 1.0f));
    glUniform3f(dLightDirection, lightDirection.x, lightDirection.y, lightDirection.z);

    GLint dLightAmbient = glGetUniformLocation(modelProgram, "dLightAmbient");
    glUniform3f(dLightAmbient, 0.1f, 0.1f, 0.1f);

    GLint dLightDiffuse = glGetUniformLocation(modelProgram, "dLightDiffuse");
    glUniform3f(dLightDiffuse, 0.5f, 0.5f, 0.5f);

    GLint pLightPosition = glGetUniformLocation(modelProgram, "pLightPosition");
    auto lightPosition =
        glm::rotate(glm::mat4(1.0f), -2.1f * currentTime, glm::vec3(0, 1, 0))
        * glm::vec4(5.0f, 5.0f, -5.0f, 1.0f);
    glUniform3f(pLightPosition, lightPosition.x, lightPosition.y, lightPosition.z);

    GLint pLightAmbient = glGetUniformLocation(modelProgram, "pLightAmbient");
    glUniform3f(pLightAmbient, 0.2f, 0.2f, 0.2f);

    GLint pLightDiffuse = glGetUniformLocation(modelProgram, "pLightDiffuse");
    glUniform3f(pLightDiffuse, 0.8f, 0.8f, 0.8f);

    glDrawElements(GL_TRIANGLES, 3 * model->GetFaceCount(), GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    glUseProgram(0);
}