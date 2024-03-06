#include <iostream>
#include <string>

#include "../Data/StringExtensions.h"
#include "../Input/Input.h"
#include "OpenGl.h"

// Can be helpful, but is really slow with the instanced rendering
bool enableDebugMode = false;

OpenGl::OpenGl()
    : capabilities() {
}

void debugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
    const GLchar* message, const void* userParam)
{
    if (StringExtensions::StartsWith(message, "API_ID_LINE_WIDTH deprecated behavior warning")) {
        // Ignore these warnings as the line width usage is for debugging logic anyways.
        return;
    }

    std::cout << message << " S: " << source << " T: " << type << " Id: " << id << " Sv: " << severity <<
        " L: " << length << std::endl;
}

bool OpenGl::Load(GraphicsConfig& config) {
    if (!glfwInit())
    {
        std::cout << "GLFW initialization failure!" << std::endl;
        return false;
    }

    // 24 depth bits, 8 stencil bits, 8x AA, major version 4.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, config.openGlMajor);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, config.openGlMinor);
    glfwWindowHint(GLFW_DEPTH_BITS, config.depthBits);
    glfwWindowHint(GLFW_STENCIL_BITS, config.stencilBits);
    glfwWindowHint(GLFW_SAMPLES, config.antialiasingLevel);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    if (enableDebugMode) {
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    }

    window = glfwCreateWindow(config.width, config.height, config.title.c_str(), nullptr, nullptr);
    if (!window) {
        std::cout << "Could not create the GLFW window!" << std::endl;
        return false;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    // Setup GLEW
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cout << "GLEW startup failure: " << err << std::endl;
        return false;
    }

    if (enableDebugMode) {
        if (GLEW_ARB_debug_output) {
            std::cout << "Can use debug output!" << std::endl;

            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            glDebugMessageCallback(debugCallback, NULL);

            glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
        }
        else {
            std::cout << "No debug mode found for this hardware! Will not enable debug logging" << std::endl;
        }
    }


    // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // Enable alpha blending
    // Necessary for IMGui displays
    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Enable line, but not polygon smoothing.
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);

    // Multisample if available
    glEnable(GL_MULTISAMPLE);

    // Let OpenGL shaders determine point sizes.
    glEnable(GL_PROGRAM_POINT_SIZE);

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    // Initialize capabilities (and log what is *actually* set by OpenGL).
    capabilities.Initialize();

    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cout << "OpenGl Init: " << error << std::endl;
        return false;
    }

    return true;
}

void OpenGl::Unload() {
    glfwDestroyWindow(window);
    window = nullptr;
}

GLFWwindow* OpenGl::GetWindow() {
    return window;
}

const OpenGlCapabilities& OpenGl::GetCapabilities() const {
    return capabilities;
}

void OpenGl::DisplayFrame() {
    glfwSwapBuffers(window);
}
