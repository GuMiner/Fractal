#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <thread>
#include <nlohmann/json.hpp>
#include <imgui.h>

#include "Telemetry/Logger.h"
#include "ScreenshotTaker.h"
#include "Input/Input.h"
#include "Data/Config/Config.h"
#include "Time.h"
#include "Tests/Experimental.h"
#include "Preprocessor/GamePreprocessor.h"
#include "Sim.h"

using json = nlohmann::json;

bool runExperiments = false;
bool preprocess = false;

// TODO need to figure out why this causes a DLL loading crash and doesn't work.
//#ifdef _WIN32
//#include <windows.h>
//extern "C" __declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
//extern "C" __declspec(dllexport) DWORD AmdPowerXpressRequestHighPerformance = 0x00000001;
//#endif

void Sim::SetupDiagnostics() {
    std::ifstream f("Config/diagnostics.json");
    DiagnosticsConfig config = json::parse(f).template get<DiagnosticsConfig>();

    // TODO drawing area for diagnostics. 
}

Sim::Sim() : fpsCounter(nullptr), threadProcessor(nullptr), filler(nullptr),
    shaderFactory(nullptr)
{
    Time::GlobalTime = new Time();

    Logger::Setup();

    // For debugging
    if (runExperiments) {
        Experimental::Test();
    }

    if (preprocess) {
        GamePreprocessor preprocessor;
        preprocessor.Process();
    }
}


bool Sim::Init() {
    // Create compatible OpenGL window
    std::ifstream f("Config/graphics.json");
    GraphicsConfig config = json::parse(f).template get<GraphicsConfig>();
    opengl = new OpenGl();
    opengl->Load(config);

    Input::Setup(opengl->GetWindow(), config);
    ImGui::CreateContext();
    if (ImGui::GetIO().Fonts->AddFontFromFileTTF("Config/Fonts/DejaVuSans.ttf", 15.f) == nullptr) {
        std::cout << "Unable to load the custom font for IMGUI to use." << std::endl;
        return false;
    }

    shaderFactory = new ShaderFactory();
    guiRenderer = new ImguiRenderer();
    if (!guiRenderer->LoadImGui(opengl->GetWindow(), shaderFactory)) {
        std::cout << "Unable to load IM GUI!" << std::endl;
        return false;
    }

    SetupDiagnostics();
    fpsCounter = new FpsCounter();
    threadProcessor = new ThreadProcessor();
    filler = new Random2DFiller(12345);
    // TODO setup thread processor with some test operation.


    testScene = new Scene();
    if (!testScene->Init(shaderFactory)) {
        Logger::LogError("Failed to load the test scene");
        return false;
    }

    return true;
}

Sim::~Sim() {
    delete Time::GlobalTime;

    delete fpsCounter;
    delete threadProcessor;
    delete filler;

    delete shaderFactory;

    delete testScene;
    delete opengl;
    Logger::Shutdown();
}

bool wireframe = false; // TODO move to debug class
void Sim::Update() {
    /**
    sf::Vector2u textureSize = simTexture.getSize();
    sf::Uint8* pixels = new sf::Uint8[textureSize.x * textureSize.y * 4]; // * 4 because pixels have 4 components (RGBA)
    
    // Multiple thread version
    threadProcessor->StartThreads(textureSize.x, textureSize.y, pixels);
    threadProcessor->WaitForThreads();

    // Single threaded version
    // filler->Fill(0, textureSize.y, textureSize.x, pixels);
    
    // for (unsigned int y = 0; y < textureSize.y; y++)
    // {
    //     for (unsigned int x = 0; x < textureSize.x; x++)
    //     {
    //         int pixelIdx = (x + y * textureSize.x) * 4;
    //         pixels[pixelIdx] = 0;
    //         pixels[pixelIdx + 1] = 255;
    //         pixels[pixelIdx + 2] = 0;
    //     }
    // }

    simTexture.update(pixels);
    delete[] pixels;
    **/

    if (Input::IsKeyPressed(GLFW_KEY_R)) {
        wireframe = !wireframe;
        glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
    }

    fpsCounter->Update();
    testScene->Update();
    guiRenderer->Update(Time::GlobalTime->RunTime(), Time::GlobalTime->LastFrameInterval());
}

void Sim::Render() {
    testScene->RenderScene();

    fpsCounter->Render();
    guiRenderer->Render();
    opengl->DisplayFrame();
}
 
void Sim::Run() {
    while (!glfwWindowShouldClose(opengl->GetWindow())) {
        glfwPollEvents();

        bool isPaused = false;
        Time::GlobalTime->Update(isPaused);

        Update();
        Render();

        // if (state.IsCaptureRequested()) {
        //     ScreenshotTaker::Take(window);
        // }
    }
}

int main() {
    Sim sim;
    if (!sim.Init()) {
        return 1;
    }

    sim.Run();
    return 0;
}