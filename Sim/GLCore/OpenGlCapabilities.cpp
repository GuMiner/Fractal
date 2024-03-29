#include <algorithm>
#include <iostream>
#include <sstream>
#include <thread>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "OpenGlCapabilities.h"

bool verboseRenderLogs = false;

void OpenGlCapabilities::GetRendererCapabilities() {
    vendor = std::string((const char*)glGetString(GL_VENDOR));
    renderer = std::string((const char*)glGetString(GL_RENDERER));
    version = std::string((const char*)glGetString(GL_VERSION));

    int extensionCount = 0;
    glGetIntegerv(GL_NUM_EXTENSIONS, &extensionCount);
    for (int i = 0; i < extensionCount; i++) {
        extensions.push_back(std::string((const char*)glGetStringi(GL_EXTENSIONS, i)));
    }
}

void OpenGlCapabilities::GetTextureCapabilities() {
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maxCombinedTextureImageUnits);
    glGetIntegerv(GL_MAX_TEXTURE_COORDS, &maxTextureCoordinateSets);
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);
}

void OpenGlCapabilities::GetUniformCapabilities() {
    glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_BLOCKS, &maxFragmentUniformBlocks);
    glGetIntegerv(GL_MAX_VERTEX_UNIFORM_BLOCKS, &maxVertexUniformBlocks);
    glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &maxUniformBlockSize);
}

void OpenGlCapabilities::Initialize() {
    logicalCoreCount = (int)std::thread::hardware_concurrency();

    GetRendererCapabilities();
    GetTextureCapabilities();
    GetUniformCapabilities();

    LogRetrievedInformation();

    logicalCoreCount = std::max(logicalCoreCount, MinCoreCount);
}

void OpenGlCapabilities::LogRetrievedInformation() {
    // Renderer
    std::cout << "Display OpenGL vendor: " << vendor << ", renderer: " << renderer
        << ", version: " << version << std::endl;


    std::cout << "  CPU Cores: " << logicalCoreCount << std::endl;

    if (verboseRenderLogs) {
        std::cout << "Extensions:" << std::endl;
        for (const std::string& extension : extensions)
        {
            std::cout << "  " << extension << std::endl;
        }
    }

    // Texture
    std::cout << "Max Texture Image units: " << maxCombinedTextureImageUnits <<
        ", coordinate sets: " << maxTextureCoordinateSets << ", texture size: " << maxTextureSize << std::endl; 

    // Uniforms
    std::cout << "Max Uniform Size: " << maxUniformBlockSize << ", max Vertex Uniform Blocks: " <<
        maxVertexUniformBlocks << ", Max Fragment Uniform Blocks: " << maxFragmentUniformBlocks << std::endl; 

}

std::string OpenGlCapabilities::GetPlatform() {
    std::stringstream platform;
    platform << vendor << "_" << renderer;
    return platform.str();
}

int OpenGlCapabilities::GetMaxTextureUnits() {
    return std::max(maxCombinedTextureImageUnits, maxTextureCoordinateSets);
}
