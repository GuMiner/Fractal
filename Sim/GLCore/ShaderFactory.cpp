#include <algorithm>
#include <sstream>
#include <iostream>
#include <gl/glew.h>
#include <vector>
#include <glm\gtx\color_space.hpp>
#include "../Telemetry/Logger.h"
#include "../Data/StringExtensions.h"
#include "ShaderFactory.h"

ShaderFactory::ShaderFactory()
    : shaderPrograms()
{
}

void ShaderFactory::LogGraphicsSettings()
{
    Logger::Log("OpenGL vendor: ", glGetString(GL_VENDOR), ", version ", glGetString(GL_VERSION), ", renderer ", glGetString(GL_RENDERER));
   // Logger::Log("OpenGL extensions: ", glGetString(GL_EXTENSIONS));

    GLint maxTextureUnits, maxUniformBlockSize;
    GLint maxVertexUniformBlocks, maxFragmentUniformBlocks;
    glGetIntegerv(GL_MAX_TEXTURE_UNITS, &maxTextureUnits);
    glGetIntegerv(GL_MAX_VERTEX_UNIFORM_BLOCKS, &maxVertexUniformBlocks);
    glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_BLOCKS, &maxFragmentUniformBlocks);
    glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &maxUniformBlockSize);

    Logger::Log("Max Texture Units: ", ", Max Uniform Size: ", (maxUniformBlockSize / 1024), " kB");
    Logger::Log("Max Vertex Uniform Blocks: ", maxVertexUniformBlocks, ", Max Fragment Uniform Blocks: ", maxFragmentUniformBlocks);
}

bool ShaderFactory::InitCore()
{
    // TODO move elsewhere
    GLenum err = glewInit();
    if (err != GLEW_OK)
    {
        Logger::LogError("GLEW startup failure: ", err, ".");
        return false;
    }

    LogGraphicsSettings();

    // TODO move elsewhere
    // Alpha blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Enable line and polygon smoothing
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);

    // Multisample if available
    glEnable(GL_MULTISAMPLE);

    // Let OpenGL shaders determine point sizes.
    glEnable(GL_PROGRAM_POINT_SIZE);

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);

    // Cutout faces that are hidden by other faces.
    // TODO re-enable once the background is removed.
    glDisable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &rainbowTexture);
    
    std::vector<glm::vec3> gradient;
    int steps = 256;
    for (int i = 0; i < steps; i++)
    {
        // Full HSV goes from 0 to 260. We stop at mid-purple.
        float hue = 320.0f * ((float)i / (float)steps);
        gradient.push_back(glm::rgbColor(glm::vec3(hue, 1.0f, 1.0f)));
    }


    glBindTexture(GL_TEXTURE_1D, rainbowTexture);
    glTexStorage1D(GL_TEXTURE_1D, 1, GL_RGB32F, steps);
    glTexSubImage1D(GL_TEXTURE_1D, 0, 0, steps, GL_RGB, GL_FLOAT, &gradient[0]);
    
    std::vector<glm::vec2> ccwQuad;
    ccwQuad.push_back(glm::vec2(-1, 1));
    ccwQuad.push_back(glm::vec2(1, -1));
    ccwQuad.push_back(glm::vec2(1, 1));
    ccwQuad.push_back(glm::vec2(-1, -1));
    ccwQuad.push_back(glm::vec2(1, -1));
    ccwQuad.push_back(glm::vec2(-1, 1));
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glEnableVertexAttribArray(0);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 2, GL_FLOAT, false, 0, nullptr);

    //2 -- 2 floats / vertex. 4 -- float32
    glBufferData(GL_ARRAY_BUFFER, 6 * 2 * 4, &ccwQuad[0], GL_STATIC_DRAW);

    glBindTexture(GL_TEXTURE_1D, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    GLenum error = glGetError();
    if (error != GL_NO_ERROR)
    {
        std::cout << "Init: " << error << std::endl;
    }

    return true;
}

void ShaderFactory::RunTestProgram(GLuint programId, float currentTime)
{
    glUseProgram(programId);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindTexture(GL_TEXTURE_1D, rainbowTexture);
    
    GLint fractalGradientLoc = glGetUniformLocation(programId, "fractalGradient");
    glUniform1i(fractalGradientLoc, 0); //  Texture location - GL_TEXTURE0
   
    GLint maxIterationsLoc = glGetUniformLocation(programId, "maxIterations");
    glUniform1i(maxIterationsLoc, 400);
    
    GLint time = glGetUniformLocation(programId, "time");
    glUniform1f(time, currentTime * 100.0f);
    
    GLint mousePos = glGetUniformLocation(programId, "c");
    glUniform2f(mousePos, -0.55f, 0.75f);

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(0);
    glUseProgram(0);

    GLenum error = glGetError();
    if (error != GL_NO_ERROR)
    {
        std::cout << "Program Error:" << error << std::endl;
    }
}

bool ShaderFactory::ReadShader(const char* rootName, const char* extension, std::string* readShader)
{
    std::stringstream filenameStream;
    filenameStream << rootName << extension;

    if (!StringExtensions::LoadStringFromFile(filenameStream.str().c_str(), *readShader))
    {
        Logger::LogError("Could not load ", extension, " shader: ", *readShader, "!");
        return false;
    }

    // Implement a non-standard #include definition to avoid shader duplication.
    std::vector<std::string> lines;
    StringExtensions::Split(*readShader, StringExtensions::Newline, true, lines);
    for (int i = lines.size() - 1; i >= 0; i--)
    {
        if (StringExtensions::StartsWith(lines[i], "#include "))
        {
            std::string includeFileName;
            if (!StringExtensions::SplitAndGrabSecondary(lines[i], includeFileName))
            {
                Logger::LogError("Could not split ", includeFileName, " from shader ", rootName, " for include parsing!");
                return false;
            }

            // Only allow (and remove) double quotes.
            includeFileName.erase(std::remove(includeFileName.begin(), includeFileName.end(), '"'), includeFileName.end());

            std::stringstream includeFilenameStream;
            includeFilenameStream  << includeFileName;

            std::string shaderIncludeFile;
            if (!StringExtensions::LoadStringFromFile(includeFilenameStream.str().c_str(), shaderIncludeFile))
            {
                Logger::LogError("Could not load ", includeFileName, "as part of ", rootName, "!");
                return false;
            }

            // Drop in our include file.
            auto erasalPosition = lines.erase(lines.begin() + i);
            lines.insert(erasalPosition, shaderIncludeFile);
        }
    }

    std::stringstream resultStream;
    for (std::string line : lines)
    {
        resultStream << line << std::endl;
    }

    *readShader = resultStream.str();

    return true;
}

// Creates and compiles a new shader of the specified type; returns true on success.
bool ShaderFactory::CreateShader(GLenum shaderType, const char* shaderSource, GLuint* shaderId)
{
    GLint compileStatus;
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderSource, NULL);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
    if (!compileStatus)
    {
        GLint logLength;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);

        std::stringstream logStream;
        char* shaderLog = new char[logLength];
        glGetShaderInfoLog(shader, logLength, &logLength, &shaderLog[0]);
        logStream << "GLSL compilation error: " << shaderLog << ".";

        Logger::LogError(logStream.str().c_str());
        delete[] shaderLog;
        return false;
    }

    *shaderId = shader;
    return true;
}

// Creates a new shader program, regardless of the number of shader stages. Stages are added in order of specification in the vector.
bool ShaderFactory::CreateProgram(std::vector<GLuint> shaders, GLuint* program)
{
    // Create the program
    GLint compileStatus;
    *program = glCreateProgram();
    for (unsigned int i = 0; i < shaders.size(); i++)
    {
        glAttachShader(*program, shaders[i]);
    }

    glLinkProgram(*program);
    glGetProgramiv(*program, GL_LINK_STATUS, &compileStatus);
    if (!compileStatus)
    {
        GLint logLength;
        glGetProgramiv(*program, GL_INFO_LOG_LENGTH, &logLength);

        char* buffer = new char[logLength];
        glGetProgramInfoLog(*program, logLength, &logLength, buffer);
        Logger::LogError("GLSL program compilation error: ", buffer);
        delete[] buffer;

        return false;
    }

    // These are auto-deleted when the program is deleted
    for (unsigned int i = 0; i < shaders.size(); i++)
    {
        glDeleteShader(shaders[i]);
    }

    return true;
}

// Creates a basic vertex-fragment shader program and adds it to the list of programs that will be deleted at the end of program operation
bool ShaderFactory::CreateShaderProgram(const char* rootName, GLuint* programId)
{
    std::string vsShader, fsShader;
    if (!ReadShader(rootName, ".vs", &vsShader) || !ReadShader(rootName, ".fs", &fsShader))
    {
        return false;
    }

    GLuint vertexShader, fragmentShader;
    if (!CreateShader(GL_VERTEX_SHADER, vsShader.c_str(), &vertexShader) || !CreateShader(GL_FRAGMENT_SHADER, fsShader.c_str(), &fragmentShader))
    {
        return false;
    }

    std::vector<GLuint> shaders;
    shaders.push_back(vertexShader);
    shaders.push_back(fragmentShader);

    if (!CreateProgram(shaders, programId))
    {
        return false;
    }

    shaderPrograms.push_back(*programId);
    return true;
}

// Creates a vertex-geomtry-shader program.
bool ShaderFactory::CreateShaderProgramWithGeometryShader(const char* rootName, GLuint* programId)
{
    std::string vsShader, gsShader, fsShader;
    if (!ReadShader(rootName, ".vs", &vsShader) || !ReadShader(rootName, ".gs", &gsShader) || !ReadShader(rootName, ".fs", &fsShader))
    {
        return false;
    }

    GLuint vertexShader, geometryShader, fragmentShader;
    if (!CreateShader(GL_VERTEX_SHADER, vsShader.c_str(), &vertexShader)
        || !CreateShader(GL_GEOMETRY_SHADER, gsShader.c_str(), &geometryShader)
        || !CreateShader(GL_FRAGMENT_SHADER, fsShader.c_str(), &fragmentShader))
    {
        return false;
    }

    std::vector<GLuint> shaders;
    shaders.push_back(vertexShader);
    shaders.push_back(geometryShader);
    shaders.push_back(fragmentShader);

    if (!CreateProgram(shaders, programId))
    {
        return false;
    }

    shaderPrograms.push_back(*programId);
    return true;
}

// Creates a shader program with vertex-tesselation_control-tesselation_evaluation-fragment stage sequence.
bool ShaderFactory::CreateShaderProgramWithTesselation(const char* rootName, GLuint* programId)
{
    std::string vsShader, tcShader, teShader, fsShader;
    if (!ReadShader(rootName, ".vs", &vsShader) || !ReadShader(rootName, ".tc", &tcShader)
        || !ReadShader(rootName, ".te", &teShader) || !ReadShader(rootName, ".fs", &fsShader))
    {
        return false;
    }

    GLuint vertexShader, tesselationControlShader, tesselationEvaluationShader, fragmentShader;
    if (!CreateShader(GL_VERTEX_SHADER, vsShader.c_str(), &vertexShader)
        || !CreateShader(GL_TESS_CONTROL_SHADER, tcShader.c_str(), &tesselationControlShader)
        || !CreateShader(GL_TESS_EVALUATION_SHADER, teShader.c_str(), &tesselationEvaluationShader)
        || !CreateShader(GL_FRAGMENT_SHADER, fsShader.c_str(), &fragmentShader))
    {
        return false;
    }

    std::vector<GLuint> shaders;
    shaders.push_back(vertexShader);
    shaders.push_back(tesselationControlShader);
    shaders.push_back(tesselationEvaluationShader);
    shaders.push_back(fragmentShader);

    if (!CreateProgram(shaders, programId))
    {
        return false;
    }

    shaderPrograms.push_back(*programId);
    return true;
}

// Deletes the specified shader program, removing it from the list of known programs.
void ShaderFactory::DeleteShaderProgram(GLuint program)
{
    glDeleteProgram(program);
    for (unsigned int i = 0; i < shaderPrograms.size(); i++)
    {
        if (shaderPrograms[i] == program)
        {
            shaderPrograms.erase(shaderPrograms.begin() + i);
            break;
        }
    }
}

ShaderFactory::~ShaderFactory()
{
    for (unsigned int i = 0; i < shaderPrograms.size(); i++)
    {
        glDeleteProgram(shaderPrograms[i]);
    }

    shaderPrograms.clear();
}
