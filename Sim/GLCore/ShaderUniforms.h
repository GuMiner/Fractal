#pragma once
#include <map>
#include <string>
#include <GL/glew.h>
#include <glm\vec3.hpp>
#include <glm\mat4x4.hpp>

class ShaderUniforms {
    GLuint programId;

    std::map<std::string, GLuint> uniformMapping;
    void EnsureUniformCached(const std::string& name);
public:
    ShaderUniforms();
    void SetProgramId(GLuint programId);

    void SetInt(const std::string& name, int value);
    void SetFloat(const std::string& name, float value);
    void SetVec3(const std::string& name, const glm::vec3& value);
    void SetMat4(const std::string& name, const glm::mat4& value);

    void SetMaterialLighting(
        const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, const float specularAmount);
    void SetDirectionalLighting(
        const glm::vec3& direction, const glm::vec3& ambient, const glm::vec3& diffuse);
    void SetPointOneLighting(
        const glm::vec3& position, const glm::vec3& ambient, const glm::vec3& diffuse);
};

