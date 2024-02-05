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

bool skipMipmapsEntirely = true;
    bool forceRegenerateMipMaps = false;
    bool updateEdges = false;
    bool updateCorners = false;

bool forceRegenerateTerrain = true;

TerrainTriangulator::TerrainTriangulator() {
    std::ifstream f("Config/terrain.json");
    config = json::parse(f).template get<TerrainConfig>();
}

void TerrainTriangulator::GenerateMipMaps() {
    if (skipMipmapsEntirely) {
        return;
    }

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
            if (updateEdges || updateCorners) {
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

    unsigned char* xPlus = updateEdges ? LoadMipsImage(x + 1, y) : nullptr;
    unsigned char* yPlus = updateEdges ? LoadMipsImage(x, y + 1) : nullptr;
    unsigned char* xyPlus = updateCorners ? LoadMipsImage(x + 1, y + 1) : nullptr;

    for (int i = 0; i < config.mipsLevels.size(); i++) {
        int mipsLevel = config.mipsLevels[i];

        // Copy the edges of xPlus and yPlus into the target image
        if (i == 0) {
            // TODO special-case logic for the largest image
        } else {
            int yOffset = config.GetMipsYOffset(mipsLevel);
            if (updateEdges) {
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

            if (updateCorners && xyPlus != nullptr) {
                // Only copy the corner
                int xTarget = mipsLevel + 5;
                int yTarget = 1 + yOffset;

                int xSource = 0;
                int ySource = yOffset;

                for (int channel = 0; channel < channels; channel++) {
                    imageData[(xTarget + yTarget * width) * channels + channel] =
                        xyPlus[(xSource + ySource * width) * channels + channel];
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

    if (xyPlus != nullptr) {
        stbi_image_free(xyPlus);
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

int GetHeight(int x, int y, int mipsYOffset, int width, unsigned char* imageData) {
    int yEffective = y + mipsYOffset;
    int imageCoord = (x + yEffective * width) * 4;
    int height = imageData[imageCoord] + ((int)imageData[imageCoord + 1] << 8);
    return height;
}

int GetHeightXPlus(int x, int y, int mipsYOffset, int mipsLevel, int width, unsigned char* imageData) {
    x = mipsLevel + 1; // Grab from the edge
    return GetHeight(x, y, mipsYOffset, width, imageData);
}

int GetHeightYPlus(int x, int y, int mipsYOffset, int mipsLevel, int width, unsigned char* imageData) {
    y = x;
    x = mipsLevel + 3;
    return GetHeight(x, y, mipsYOffset, width, imageData);
}

int GetHeightXOrYPlus(int x, int y, int mipsYOffset, int mipsLevel, int width, unsigned char* imageData) {
    if (x == mipsLevel && y == mipsLevel) {
        // Get the special corner pixel
        x = mipsLevel + 5;
        y = 1;
        return GetHeight(x, y, mipsYOffset, width, imageData);
    }
    else if (x == mipsLevel) {
        // Can assume y+ is fine, so use existing logic
        return GetHeightXPlus(x, y, mipsYOffset, mipsLevel, width, imageData);
    }
    else if (y == mipsLevel) {
        // Can assume x+ is fine, so use existing logic
        return GetHeightYPlus(x, y, mipsYOffset, mipsLevel, width, imageData);
    }

    // Neither X+ or Y+ exceed the image, so return the height directly
    return GetHeight(x, y, mipsYOffset, width, imageData);
}

void TerrainTriangulator::TriangulateTile(int x, int y, std::string outputFile, int mipsLevel) {
    unsigned char* imageData = LoadMipsImage(x, y);

    int width = config.mipsLevels[0];

    std::vector<glm::vec3> vertices;
    std::vector<glm::ivec3> faces;

    // Reuse vertices, at the cost of needing to compute face normals instead of vertex normals for flat shading
    // This simplifies tile corners because averaged vertex normals don't align correctly
    int mipsYOffset = config.GetMipsYOffset(mipsLevel);
    for (int y = 0; y < mipsLevel + 1; y++) {
        for (int x = 0; x < mipsLevel + 1; x++) {
            int height = GetHeightXOrYPlus(x, y, mipsYOffset, mipsLevel, width, imageData);
            vertices.push_back(glm::vec3(x, y, height));

            if (x < mipsLevel && y < mipsLevel) {
                int vertex = x + y * (mipsLevel + 1);
                int vertexXPlus = (x + 1) + y * (mipsLevel + 1);
                int vertexYPlus = x + (y + 1) * (mipsLevel + 1);
                int vertexXYPlus = (x + 1) + (y + 1) * (mipsLevel + 1);
                faces.push_back(glm::ivec3(vertex, vertexYPlus, vertexXYPlus));
                faces.push_back(glm::ivec3(vertex, vertexXYPlus, vertexXPlus));
            }
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