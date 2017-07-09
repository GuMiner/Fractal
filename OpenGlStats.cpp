#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "logging\Logger.h"
#include "strings\StringUtils.h"
#include "OpenGlStats.h"

void OpenGlStats::LogStats()
{
    Logger::Log("OpenGL vendor: ", glGetString(GL_VENDOR), ", version ", glGetString(GL_VERSION), ", renderer ", glGetString(GL_RENDERER));
    Logger::LogDebug("OpenGL extensions: ", glGetString(GL_EXTENSIONS));

    GLint maxTextureUnits, maxUniformBlockSize;
    GLint maxVertexUniformBlocks, maxFragmentUniformBlocks;
    GLint maxTextureSize;
    glGetIntegerv(GL_MAX_TEXTURE_UNITS, &maxTextureUnits);
    glGetIntegerv(GL_MAX_VERTEX_UNIFORM_BLOCKS, &maxVertexUniformBlocks);
    glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_BLOCKS, &maxFragmentUniformBlocks);
    glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &maxUniformBlockSize);
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);

    Logger::Log("Max Texture Units: ", maxTextureUnits, ", Max Uniform Size: ", StringUtils::GetNiceByteValue(maxUniformBlockSize));
    Logger::Log("Max Vertex Uniform Blocks: ", maxVertexUniformBlocks, ", Max Fragment Uniform Blocks: ", maxFragmentUniformBlocks);
    Logger::Log("Max Texture Size: ", maxTextureSize);
}
