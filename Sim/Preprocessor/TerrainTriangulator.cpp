#include <direct.h>
#include <io.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <ostream>
#include <stb_image.h>
#include <stb_image_resize.h>
#include <stb_image_write.h>
#include <glm\vec3.hpp>
#include "../Data/BinaryModel.h"
#include "TerrainTriangulator.h"

bool forceRegenerateMipMaps = false;
bool updateEdges = true;

bool forceRegenerateTerrain = true;

TerrainTriangulator::TerrainTriangulator() {
    std::ifstream f("Config/terrain.json");
    config = json::parse(f).template get<TerrainConfig>();
}

void TerrainTriangulator::GenerateMipMaps() {
    for (int x = 0; x < config.width; x++) {
        std::cout << "MipMaps: Processing " << x << std::endl;
        for (int y = 0; y < config.height; y++) {
            std::stringstream inputFile;
            inputFile << "Config/Terrain/Source/" << y << "/" << x << ".png";

            std::stringstream outputFilePath;
            SetMipMapOutputFilePath(&outputFilePath, x, y);
            if (forceRegenerateMipMaps || !FileExists(outputFilePath.str())) {
                std::cout << "  Reducing " << x << ", " << y << std::endl;

                ReduceTile(inputFile.str().c_str(), outputFilePath.str().c_str());
            }
        }
    }

    for (int x = 0; x < config.width; x++) {
        std::cout << "MipMaps: Edge copying " << x << std::endl;
        for (int y = 0; y < config.height; y++) {
            if (updateEdges) {
                std::cout << " Updating " << x << ", " << y << std::endl;

                CopyEdges(x, y);
            }
        }
    }
}

void TerrainTriangulator::ReduceTile(const char* inputFile, const char* outputFile) {
    int width = 0;
    int height = 0;
    int channels = 0;
    unsigned char* imageData = stbi_load(inputFile, &width, &height, &channels, STBI_rgb_alpha);

    // No matter the input (should always be ~1000x1000), create MIPs from that
    // Oriented top to bottom

    int mipmapWidth = 512;
    int fullStride = 512 * 4;
    int yOffset = 0;
    int lastOffset = -1;

    int mipmapHeight = config.TotalMipsHeight();
    unsigned char* mipmapData = new unsigned char[mipmapWidth * mipmapHeight * 4];
    for (int i = 0; i < config.mipsLevels.size(); i++) {
        int mipsLevel = config.mipsLevels[i];
        if (i == 0) {
            stbir_resize_uint8(
                imageData, width, height, 0,
                mipmapData + yOffset, mipsLevel, mipsLevel, 0, 4);
        }
        else {
            // Create each mipmap from the previous one to speed up the process
            stbir_resize_uint8(
                mipmapData + lastOffset, width, height, fullStride,
                mipmapData + yOffset, mipsLevel, mipsLevel, fullStride, 4);
        }

        width = mipsLevel;
        height = mipsLevel;
        lastOffset = yOffset;
        yOffset += mipsLevel * fullStride;
    }

    stbi_write_png(outputFile, mipmapWidth, mipmapHeight, 4, mipmapData, 0);
    stbi_image_free(imageData);
    delete[] mipmapData;
}

unsigned char* TerrainTriangulator::LoadMipsImage(int x, int y) {
    if (x == -1 || x >= config.width || y == -1 || y >= config.height) {
        return nullptr;
    }

    std::stringstream targetFile;
    targetFile << "Config/Terrain/Generated/" << y << "/" << x << "-mips.png";

    int width = 0;
    int height = 0;
    int channels = 0;
    unsigned char* imageData = stbi_load(targetFile.str().c_str(), &width, &height, &channels, STBI_rgb_alpha);
    return imageData;
}

void TerrainTriangulator::CopyEdges(int x, int y) {
    std::stringstream targetFile;
    targetFile << "Config/Terrain/Generated/" << y << "/" << x << "-mips.png";

    int width = 0;
    int height = 0;
    int channels = 0;
    unsigned char* imageData = stbi_load(targetFile.str().c_str(), &width, &height, &channels, STBI_rgb_alpha);

    unsigned char* xPlus = LoadMipsImage(x + 1, y);
    unsigned char* yPlus = LoadMipsImage(x, y + 1);

    for (int i = 0; i < config.mipsLevels.size(); i++) {
        int mipsLevel = config.mipsLevels[i];

        // Copy the edges of xPlus and yPlus into the target image
        if (i == 0) {
            // TODO special-case logic for the largest image
        } else {
            int yOffset = config.GetMipsYOffset(mipsLevel);
            if (xPlus != nullptr) {
                int xSource = 0;
                int ySource = yOffset;
                int xTarget = mipsLevel + 1;
                int yTarget = yOffset;

                for (int i = 0; i < mipsLevel; i++) {
                    for (int channel = 0; channel < channels; channel++) {
                        imageData[(xTarget + yTarget * width) * channels + channel] =
                            xPlus[(xSource + ySource * width) * channels + channel];
                    }

                    ySource++;
                    yTarget++;
                }
            }

            if (yPlus != nullptr) {
                int xSource = 0;
                int ySource = yOffset;
                int xTarget = mipsLevel + 3; // aka, mips-image + <buffer> + edge
                int yTarget = yOffset;
                
                for (int i = 0; i < mipsLevel; i++) {
                    for (int channel = 0; channel < channels; channel++) {
                        imageData[(xTarget + yTarget * width) * channels + channel] =
                            yPlus[(xSource + ySource * width) * channels + channel];
                    }

                    // Saved with left-to-right mapped to top-to-bottom
                    xSource++;
                    yTarget++;
                }
            }
        }
    }


    stbi_write_png(targetFile.str().c_str(), width, height, 4, imageData, 0);
    stbi_image_free(imageData);

    if (xPlus != nullptr) {
        stbi_image_free(xPlus);
    }

    if (yPlus != nullptr) {
        stbi_image_free(yPlus);
    }
}

void TerrainTriangulator::TriangulateTerrain() {
    for (int x = 0; x < config.width; x++) {
        std::cout << "Triangulation: Processing " << x << std::endl;
        for (int y = 0; y < config.height; y++) {
            std::stringstream inputFile;
            inputFile << config.generatedPath << y << "/" << x << "-mips.png";

            int testMipsLevels[] = { 8 };
            for (int mipsLevel : testMipsLevels) { //config.mipsLevels) {
                std::stringstream outputFilePath;
                SetOutputFilePath(&outputFilePath, x, y, mipsLevel);
                if (forceRegenerateTerrain || !FileExists(outputFilePath.str())) {
                    std::cout << "  Triangulating " << x << ", " << y << "(" << mipsLevel << ")" << std::endl;
                    TriangulateTile(x, y, outputFilePath.str(), mipsLevel);
                }
            }
        }
    }
}

void TerrainTriangulator::TriangulateTile(int x, int y, std::string outputFile, int mipsLevel) {
    unsigned char* imageData = LoadMipsImage(x, y);
    int width = config.mipsLevels[0];

    std::vector<glm::vec3> vertices;
    std::vector<glm::ivec3> faces;

    int mipsYOffset = config.GetMipsYOffset(mipsLevel);
    for (int x = 0; x < mipsLevel; x++) {
        for (int y = 0; y < mipsLevel; y++) {
            int xEffective = x;
            int yEffective = y + mipsYOffset;

            int imageCoord = (xEffective + yEffective * width) * 4;
            int height = imageData[imageCoord] + ((int)imageData[imageCoord + 1] << 8); // TODO might need modifications
            
            // TODO needs to be able to load other images and get edges from that. 
            // This probably could be done by re-mip-mapping the images, or done here
            int xPlus1 = x == mipsLevel - 1 ? x : x + 1;
            int yPlus1 = (y == mipsLevel - 1 ? y : y + 1) + mipsYOffset;

            int imageCoordX = (xPlus1 + yEffective * width) * 4;
            int imageCoordY = (xEffective + yPlus1 * width) * 4;
            int imageCoordXY = (xPlus1 + yPlus1 * width) * 4;
            int heightX = imageData[imageCoordX] + ((int)imageData[imageCoordX + 1] << 8);
            int heightY = imageData[imageCoordY] + ((int)imageData[imageCoordY + 1] << 8);
            int heightXY = imageData[imageCoordXY] + ((int)imageData[imageCoordXY + 1] << 8);

            // V1 -- just flat plane, connectors TBD
            // TODO can reuse vertices now, saving lots of 
            int vs = vertices.size();
            faces.push_back(glm::ivec3(vs, vs + 1, vs + 2)); // CW faces of the top plane
            faces.push_back(glm::ivec3(vs, vs + 2, vs + 3));
            vertices.push_back(glm::vec3(x, y, height));
            vertices.push_back(glm::vec3(x, y + 1, heightY));
            vertices.push_back(glm::vec3(x + 1, y + 1, heightXY));
            vertices.push_back(glm::vec3(x + 1, y, heightX));
        }
    }

    if (!BinaryModel::Save(outputFile, vertices, faces)) {
        std::cout << "Unable to save " << outputFile << "!" << std::endl;
    }

    stbi_image_free(imageData);
}

void TerrainTriangulator::SetOutputFilePath(std::stringstream* outputFilePath, int x, int y, int mipsLevel) {
    *outputFilePath << "Config/Terrain/Generated/" << y;
    EnsureFolderExists(outputFilePath->str());

    *outputFilePath << "/" << x << "-" << mipsLevel << ".off";
}

void TerrainTriangulator::SetMipMapOutputFilePath(std::stringstream* outputFilePath, int x, int y) {
    *outputFilePath << "Config/Terrain/Generated/" << y;
    EnsureFolderExists(outputFilePath->str());

    *outputFilePath << "/" << x << "-mips.png";
}

void TerrainTriangulator::EnsureFolderExists(const std::string& folder) {
    if (_access(folder.c_str(), 0) != 0)
    {
        _mkdir(folder.c_str());
    }
}

bool TerrainTriangulator::FileExists(const std::string& fileName) {
    std::ifstream infile(fileName);
    return infile.good();
}