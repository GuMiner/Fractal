#include "logging\Logger.h"
#include "Input.h"
#include "OpenGl.h"

OpenGl::OpenGl()
    : capabilities(), performanceProfiler()
{
}

bool OpenGl::Load(Viewer* viewer)
{
    // 24 depth bits, 8 stencil bits, 8x AA, major version 4.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_DEPTH_BITS, 16);
    glfwWindowHint(GLFW_STENCIL_BITS, 16);
    glfwWindowHint(GLFW_SAMPLES, 8);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(viewer->ScreenWidth, viewer->ScreenHeight, "Fractal", nullptr, nullptr);
    if (!window)
    {
        Logger::LogError("Could not create the GLFW window!");
        return false;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    Input::Setup(window, viewer);

    // Setup GLEW
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK)
    {
        Logger::LogError("GLEW startup failure: ", err, ".");
        return false;
    }

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // Enable alpha blending
    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Enable line, but not polygon smoothing.
    glEnable(GL_LINE_SMOOTH);

    // Let OpenGL shaders determine point sizes.
    glEnable(GL_PROGRAM_POINT_SIZE);

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    // Initialize capabilities (and log what is *actually* set by OpenGL).
    capabilities.Initialize();

    return true;
}

void OpenGl::Unload()
{
    glfwDestroyWindow(window);
    window = nullptr;
}

GLFWwindow* OpenGl::GetWindow()
{
    return window;
}

const OpenGlCapabilities & OpenGl::GetCapabilities() const
{
    return capabilities;
}

void OpenGl::PerformPerformanceAnalysis()
{
    performanceProfiler.Analyze();
}

IPerformanceProfiler* OpenGl::GetPerformanceProfiler()
{
    return &performanceProfiler;
}

void OpenGl::DisplayFrame()
{
    glfwSwapBuffers(window);
}
