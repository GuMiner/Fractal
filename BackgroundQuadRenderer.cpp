#include <vector>
#include "logging\Logger.h"
#include "BackgroundQuadRenderer.h"

BackgroundQuadRenderer::BackgroundQuadRenderer(IBackgroundRendererExtension* extension)
    : extension(extension)
{
}

bool BackgroundQuadRenderer::Init(ShaderFactory* shaderFactory, std::string shaderName)
{
    Logger::Log("Loading the ", shaderName, " rendering shading program for full-quad background rendering...");
    if (!shaderFactory->CreateShaderProgram(shaderName.c_str(), &programId))
    {
        Logger::LogError("Failed to load the ", shaderName, " rendering shader; cannot continue.");
        return false;
    }

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // CCW winding
    vertices.push_back(glm::vec2(-1, -1));
    vertices.push_back(glm::vec2(1, -1));
    vertices.push_back(glm::vec2(1, 1));

    vertices.push_back(glm::vec2(-1, -1));
    vertices.push_back(glm::vec2(1, 1));
    vertices.push_back(glm::vec2(-1, 1));

    glEnableVertexAttribArray(0);
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec2), &vertices[0], GL_STATIC_DRAW);

    if (extension != nullptr)
    {
        return extension->Init(programId);
    }

    return true;
}

void BackgroundQuadRenderer::Render()
{
    glUseProgram(programId);
    glBindVertexArray(vao);

    if (extension != nullptr)
    {
        extension->Render();
    }

    glDrawArrays(GL_TRIANGLES, 0, (GLsizei)vertices.size());
}

BackgroundQuadRenderer::~BackgroundQuadRenderer()
{
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);

    if (extension != nullptr)
    {
        delete extension;
    }
}
