#pragma once
#include <string>
#include <vector>

class OpenGlCapabilities
{
    void GetRendererCapabilities();
    void GetTextureCapabilities();
    void GetUniformCapabilities();
    void LogRetrievedInformation();

public:
    // High-level capabilities
    std::string vendor;
    std::string renderer;
    std::string version;
    std::vector<std::string> shadingLanguageVersions;
    std::vector<std::string> extensions;

    // Texture capabilities
    int maxCombinedTextureImageUnits;
    int maxTextureCoordinateSets;
    int maxTextureSize;

    // Uniform capabilities
    int maxFragmentUniformBlocks;
    int maxVertexUniformBlocks;
    int maxUniformBlockSize;

    void Initialize();
    
    std::string GetPlatform();
    int GetMaxTextureUnits();
};

