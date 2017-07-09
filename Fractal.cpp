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

#pragma comment(lib, "opengl32")
#pragma comment(lib, "lib/glfw3.lib")
#pragma comment(lib, "lib/glew32.lib")
#pragma comment(lib, "lib/imgui.lib")
#pragma comment(lib, "lib/sfml-system")

Fractal::Fractal()
    : opengl(), shaderFactory(), viewer(), guiRenderer(), fpsCounter(), objectLoader()
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

    objectLoader.Update(frameTime);
    fpsCounter.UpdateFps(frameTime);
}

void Fractal::Render(glm::mat4& viewMatrix)
{
    glm::mat4 projectionMatrix = viewer.perspectiveMatrix * viewMatrix;

    // Clear the screen (and depth buffer) before any rendering begins.
    const GLfloat color[] = { 0, 0.2f, 0, 1 };
    const GLfloat one = 1.0f;
    glClearBufferfv(GL_COLOR, 0, color);
    glClearBufferfv(GL_DEPTH, 0, &one);

    objectLoader.Render(projectionMatrix);
    fpsCounter.Render();

    // Must always be last.
    guiRenderer.Render();
}

bool Fractal::LoadGraphics()
{
    if (!opengl.Load(&viewer))
    {
        return false;
    }

    if (!LineRenderer::LoadProgram(&shaderFactory))
    {
        return false;
    }

    if (ImGui::GetIO().Fonts->AddFontFromFileTTF("./fonts/DejaVuSans.ttf", 15.f) == nullptr)
    {
        Logger::LogError("Unable to load the custom font for IMGUI to use.");
        return false;
    }

    if (!guiRenderer.LoadImGui(opengl.GetWindow(), &shaderFactory))
    {
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

            Render(viewer.viewMatrix);
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