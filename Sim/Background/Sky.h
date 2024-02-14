#pragma once
#include "../GLCore/ShaderFactory.h"
#include "../Camera.h"

class Sky {
    GLuint programId;

    GLuint vao;
    GLuint vbo;

    std::vector<glm::vec3> skyBox;

public:
    bool Init(ShaderFactory* shaderFactory);
    void Render(Camera* camera);
};

