#include <algorithm>
#include <sstream>
#include <thread>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "logging\Logger.h"
#include "strings\StringUtils.h"
#include "OpenGlCapabilities.h"

void OpenGlCapabilities::GetRendererCapabilities()
{
    vendor = std::string((const char*)glGetString(GL_VENDOR));
    renderer = std::string((const char*)glGetString(GL_RENDERER));
    version = std::string((const char*)glGetString(GL_VERSION));

    int shadingLanguageCount = 0;
    glGetIntegerv(GL_NUM_SHADING_LANGUAGE_VERSIONS, &shadingLanguageCount);
    for (int i = 0; i < shadingLanguageCount; i++)
    {
        shadingLanguageVersions.push_back(std::string((const char*)glGetStringi(GL_SHADING_LANGUAGE_VERSION, i)));
    }

    int extensionCount = 0;
    glGetIntegerv(GL_NUM_EXTENSIONS, &extensionCount);
    for (int i = 0; i < extensionCount; i++)
    {
        extensions.push_back(std::string((const char*)glGetStringi(GL_EXTENSIONS, i)));
    }
}

void OpenGlCapabilities::GetTextureCapabilities()
{
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maxCombinedTextureImageUnits);
    glGetIntegerv(GL_MAX_TEXTURE_COORDS, &maxTextureCoordinateSets);
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);
}

void OpenGlCapabilities::GetUniformCapabilities()
{
    glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_BLOCKS, &maxFragmentUniformBlocks);
    glGetIntegerv(GL_MAX_VERTEX_UNIFORM_BLOCKS, &maxVertexUniformBlocks);
    glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &maxUniformBlockSize);
}

void OpenGlCapabilities::Initialize()
{
    logicalCoreCount = (int)std::thread::hardware_concurrency();

    GetRendererCapabilities();
    GetTextureCapabilities();
    GetUniformCapabilities();

    LogRetrievedInformation();

    logicalCoreCount = std::max(logicalCoreCount, MinCoreCount);
}

void OpenGlCapabilities::LogRetrievedInformation()
{
    // Renderer
    Logger::Log("Concurrency count: ", logicalCoreCount);

    Logger::Log("OpenGL vendor: ", vendor, ", renderer: ", renderer, ", version: ", version);
    Logger::Log("Shading Language Versions Supported:");
    for (const std::string& version : shadingLanguageVersions)
    {
        Logger::Log("  ", version);
    }

    Logger::Log("Extensions:");
    for (const std::string& extension : extensions)
    {
        Logger::Log("  ", extension);
    }

    // Texture
    Logger::Log("Max Texture Image units: ", maxCombinedTextureImageUnits,
        ", coordinate sets: ", maxTextureCoordinateSets, ", texture size: ", maxTextureSize);

    // Uniforms
    Logger::Log("Max Uniform Size: ", StringUtils::GetNiceByteValue(maxUniformBlockSize), ", max Vertex Uniform Blocks: ",
        maxVertexUniformBlocks, ", Max Fragment Uniform Blocks: ", maxFragmentUniformBlocks);
}

std::string OpenGlCapabilities::GetPlatform()
{
    std::stringstream platform;
    platform << vendor << "_" << renderer;
    return platform.str();
}

int OpenGlCapabilities::GetMaxTextureUnits()
{
    return std::max(maxCombinedTextureImageUnits, maxTextureCoordinateSets);
}
