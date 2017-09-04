#include <chrono>
#include <sstream>
#include <string>
#include <thread>
#include <glm\gtx\transform.hpp>
#include <imgui\imgui.h>
#include "logging\Logger.h"
#include "strings\StringUtils.h"
#include "testing\ITestable.h"
#include "Input.h"
#include "version.h"
#include "Fractal.h"

#pragma comment(lib, "opengl32") // Built-in
#pragma comment(lib, "lib/glfw3dll.lib") // v3.2.1
#pragma comment(lib, "lib/glew32.lib") // v2.1.0
#pragma comment(lib, "lib/imgui.lib") // v1.51

Fractal::Fractal()
    : opengl(), shaderFactory(), guiRenderer(), viewer(), 
      world(), fpsCounter()
{
}

bool Fractal::Initialize()
{
    // Setup GLFW
    if (!glfwInit())
    {
        Logger::LogError("GLFW startup failure");
        return false;
    }

    return true;
}

void Fractal::Deinitialize()
{
    glfwTerminate();
}

void Fractal::HandleEvents(bool& focusPaused)
{
    glfwPollEvents();
    focusPaused = !Input::hasFocus;
}

void Fractal::Update(float currentTime, float frameTime)
{
    guiRenderer.Update(currentTime, frameTime); // Must be before any IMGUI commands are passed in.
    
    viewer.Update(frameTime);
    world.Update(viewer.GetCamera().position, currentTime, frameTime);

    fpsCounter.UpdateFps(frameTime);
}

void Fractal::Render(float currentTime, glm::mat4& viewMatrix)
{
    glm::mat4 projectionMatrix = viewer.perspectiveMatrix * viewMatrix;

    // Clear the screen (and depth buffer) before any rendering begins.
    const GLfloat color[] = { 0, 0.2f, 0, 1 };
    const GLfloat one = 1.0f;
    glClearBufferfv(GL_COLOR, 0, color);
    glClearBufferfv(GL_DEPTH, 0, &one);

    world.Render(projectionMatrix);
    fpsCounter.Render();
    viewer.Render();

    // Must always be last in case other rendering steps add GUI elements.
    guiRenderer.Render();
}

bool Fractal::LoadGraphics()
{
    if (!opengl.Load(&viewer))
    {
        return false;
    }

    opengl.PerformPerformanceAnalysis();

    if (ImGui::GetIO().Fonts->AddFontFromFileTTF("./fonts/DejaVuSans.ttf", 15.f) == nullptr)
    {
        Logger::LogError("Unable to load the custom font for IMGUI to use.");
        return false;
    }

    if (!guiRenderer.LoadImGui(opengl.GetWindow(), &shaderFactory))
    {
        Logger::LogError("Unable to load IM GUI!");
        return false;
    }

    if (!world.LoadGraphics(opengl.GetCapabilities(), opengl.GetPerformanceProfiler(), &shaderFactory))
    {
        Logger::LogError("Unable to load the world graphics!");
        return false;
    }

    return true;
}

void Fractal::UnloadGraphics()
{
    guiRenderer.UnloadImGui();
    opengl.Unload();
}

bool Fractal::Run()
{
    float gameTime = 0;
    float lastFrameTime = 0.06f;

    bool focusPaused = false;

    while (!glfwWindowShouldClose(opengl.GetWindow()))
    {
        HandleEvents(focusPaused);

        // Run the game and render if not paused.
        if (!focusPaused)
        {
            Update(gameTime, lastFrameTime);

            Render(gameTime, viewer.viewMatrix);
            opengl.DisplayFrame();
        }

        float now = (float)glfwGetTime();
        lastFrameTime = (now - gameTime);
        gameTime += lastFrameTime;

        // Delay to run approximately at our maximum framerate.
        long sleepDelay = ((long)1e6 / viewer.MaxFramerate) - (long)(lastFrameTime * 1e6);
        if (sleepDelay > 0)
        {
            std::this_thread::sleep_for(std::chrono::microseconds(sleepDelay));
        }
    }

    return true;
}

int main()
{
    Logger::Setup("fractal.log", true);
    Logger::Log(AutoVersion::GetVersionString("Fractal"));

    ITestable* testables[] =
    {
        new StringUtils()
    };

    for (ITestable* testable : testables)
    {
        if (!testable->Test())
        {
            Logger::Log("Test failed!");
            return 1;
        }
    }

    Fractal* fractal = new Fractal();
    if (!fractal->Initialize())
    {
        Logger::LogError("Fractal initialization failed!");
    }
    else
    {
        Logger::Log("Basic Initialization complete!");
        if (!fractal->LoadGraphics())
        {
            Logger::LogError("Fractal graphics initialization failed!");
        }
        else
        {
            Logger::Log("Graphics Initialized!");
            if (!fractal->Run())
            {
                Logger::LogError("Fractal operation failed!");
            }

            fractal->UnloadGraphics();
        }

        fractal->Deinitialize();
    }

    delete fractal;
    Logger::Log("Done.");
    Logger::Shutdown();
    return 0;
}