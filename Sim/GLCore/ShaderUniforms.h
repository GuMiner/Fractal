#pragma
#include <map>
#include <string>
#include <GL/glew.h>
#include <glm\vec3.hpp>

class ShaderUniforms {
    GLuint programId;

    std::map<std::string, GLuint> uniformMapping;
    void EnsureUniformCached(const std::string& name);
public:
    ShaderUniforms();
    void SetProgramId(GLuint programId);

    void SetFloat(const std::string& name, float value);
    void SetVec3(const std::string& name, const glm::vec3& value);
};

