#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>

class Texture
{
    int width;
    int height;
    std::vector<unsigned char> textureData;

public:
    int activeTextureOffset;
    GLuint textureId;

    Texture();
    void SetupOpenGlTexture(int activeTextureOffset);
    void SetImageData(int width, int height, std::vector<unsigned char> textureData);
    void TransferToOpenGl();
    int GetWidth() const;
    int GetHeight() const;
    void Deinitialize();
};

