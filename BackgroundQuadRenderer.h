#pragma once
#include <string>
#include <GL/glew.h>
#include <glm\vec2.hpp>
#include "GuCommon\shaders\ShaderFactory.h"
#include "IBackgroundRendererExtension.h"

// Renders a background quad with the assumption that all rendering happens in the fragment shader.
class BackgroundQuadRenderer
{
    std::vector<glm::vec2> vertices;

    GLuint programId;
    GLuint vao;
    GLuint vbo;

    IBackgroundRendererExtension* extension;

public:
    BackgroundQuadRenderer(IBackgroundRendererExtension* extension);
    bool Init(ShaderFactory* shaderFactory, std::string shaderName);
    void Render();
    ~BackgroundQuadRenderer();
};

