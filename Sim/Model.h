#pragma once
#include <glm/vec3.hpp>
#include "GLCore/ShaderFactory.h"
#include "Camera.h"

class Model
{
    GLuint modelProgram;

    GLuint modelVao;
    GLuint positionVbo;
    GLuint normalVbo;
    GLuint indexVbo;

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::ivec3> faces;
public:
    Model();
    bool Init(ShaderFactory* shaderFactory);
    bool SendMesh();
    void Render(Camera* camera);
};