#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class IBackgroundRendererExtension
{
public:
    virtual bool Init(GLuint programId) = 0;
    virtual void Render() = 0;
    
    virtual ~IBackgroundRendererExtension()
    {
    }
};