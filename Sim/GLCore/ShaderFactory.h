#pragma once
#include <GL/glew.h>
#include <string>
#include <vector>

// Handles loading and deleting shaders from a common 'shaders' folder
class ShaderFactory
{
    bool ReadShader(const char* rootName, const char* extension, std::string* readShader);
    bool CreateShader(GLenum shaderType, const char* shaderSource, GLuint* shaderId);
    bool CreateProgram(std::vector<GLuint> shaders, GLuint* program);

    std::vector<GLuint> shaderPrograms;
public:
    ShaderFactory();
    bool CreateShaderProgram(const char* rootName, GLuint* programId);
    bool CreateShaderProgramWithGeometryShader(const char* rootName, GLuint* programId);
    bool CreateShaderProgramWithTesselation(const char* rootName, GLuint* programId);
    void DeleteShaderProgram(GLuint programId);
    ~ShaderFactory();
};

