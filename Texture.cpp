#include "Texture.h"

Texture::Texture()
{
}

void Texture::Initialize(int activeTextureOffset)
{
    this->activeTextureOffset = activeTextureOffset;
    glGenTextures(1, &textureId);
}

void Texture::SetImageData(int width, int height, std::vector<unsigned char> textureData)
{
    this->width = width;
    this->height = height;
    this->textureData = textureData;
}

void Texture::TransferToOpenGl()
{
    glActiveTexture(GL_TEXTURE0 + activeTextureOffset);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, width, height); // This API expects the *sized* format... 
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, &textureData[0]); // ... and this API does not.
}

int Texture::GetWidth() const
{
    return width;
}

int Texture::GetHeight() const
{
    return height;
}

Texture::~Texture()
{
    glDeleteTextures(1, &textureId);
}
