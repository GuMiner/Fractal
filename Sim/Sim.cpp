#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <thread>

#include "Telemetry/Logger.h"
#include "ScreenshotTaker.h"
#include "KeyboardInput.h"
#include <nlohmann/json.hpp>
#include "Data/Config/Config.h"
#include <SFML/OpenGL.hpp>

#include "Time.h"
#include "Sim.h"
#include "Tests/Experimental.h"
#include "Preprocessor/GamePreprocessor.h"

using json = nlohmann::json;

bool debugCoreOpenGl = false;
bool preprocess = false;

//#ifdef _WIN32
//#include <windows.h>
//extern "C" __declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
//extern "C" __declspec(dllexport) DWORD AmdPowerXpressRequestHighPerformance = 0x00000001;
//#endif

void Sim::SetupDiagnostics() {
    std::ifstream f("Config/diagnostics.json");
    DiagnosticsConfig config = json::parse(f).template get<DiagnosticsConfig>();
    simTexture.create(config.width, config.height);
    simTexture.setRepeated(false);
    simTexture.setSmooth(false); 

    simSprite.setTexture(simTexture);
}

Sim::Sim() : fpsCounter(nullptr), threadProcessor(nullptr), filler(nullptr),
    shaderFactory(nullptr)
{
    Time::GlobalTime = new Time();

    Logger::Setup();

    // For debugging
    if (debugCoreOpenGl) {
        Experimental::Test();
    }

    if (preprocess) {
        GamePreprocessor preprocessor;
        preprocessor.Process();
    }
}

Sim::~Sim() {
    delete Time::GlobalTime;

    delete fpsCounter;
    delete threadProcessor;
    delete filler;

    delete shaderFactory;

    delete testScene;
    Logger::Shutdown();
}


bool Sim::Init() {
    SetupDiagnostics();
    fpsCounter = new FpsCounter();
    threadProcessor = new ThreadProcessor();
    filler = new Random2DFiller(12345);
    // TODO setup thread processor with some test operation.

    shaderFactory = new ShaderFactory();

    testScene = new Scene();

    return true;
}

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

    fpsCounter->Update();
    testScene->Update();
    
}

void Sim::Render(sf::RenderWindow& window) {
    testScene->RenderScene();

    if (!debugCoreOpenGl)
    {
        window.pushGLStates();
        window.draw(simSprite);
        fpsCounter->Render(window);
        window.popGLStates();
    }
}

void Sim::UpdatePerspective(unsigned int width, unsigned int height) {
    // Letterboxing is done at the top and bottom.
    float necessaryWidth = (float)height * 1.77f; // TODO get aspect ratio from config
    if (necessaryWidth > width)
    {
        // Letterbox the top and the bottom of the screen so that the aspect ratio is met
        float effectiveHeight = (float)width / 1.77f;
        float heightDelta = ((float)height - effectiveHeight) / 2.0f;
        glViewport(0, (int)heightDelta, (GLsizei)width, (GLsizei)effectiveHeight);
    }
    else
    {
        // Letterbox the left and the right so that the aspect ratio is met.
        float widthDelta = ((float)width - necessaryWidth) / 2.0f;
        glViewport((GLint)widthDelta, (GLint)0, (GLsizei)necessaryWidth, (GLsizei)height);
    }
}

bool cursorGrabbed = false;
bool wireframe = false; // TODO move to debug class
 
void Sim::HandleEvents(sf::RenderWindow& window, SimUpdateState& state) {
    KeyboardInput::ResetMouseDelta();

    sf::Vector2i newMousePos = window.getPosition() + sf::Vector2i(window.getSize().x / 2, window.getSize().y / 2);
    if (!cursorGrabbed) {
        sf::Mouse::setPosition(newMousePos);
    }
    KeyboardInput::SetMouseCenter(glm::ivec2(newMousePos.x, newMousePos.y));

    // Handle all events.
    sf::Event event;
    while (window.pollEvent(event)) {

        // Handle generic pause and window events
        state.Update(event);

        if (event.type == sf::Event::Resized)
        {
            // UpdatePerspective(event.size.width, event.size.height);
            KeyboardInput::SetMouseCenter(glm::ivec2(event.size.width / 2, event.size.height / 2));
        }
        else if (KeyboardInput::HandleEvent(event)) {
            // Event handled, continue from here
            if (KeyboardInput::IsKeyPressed(sf::Keyboard::Key::R)) {
                wireframe = !wireframe;
                glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
            }
            else if (KeyboardInput::IsKeyPressed(sf::Keyboard::Key::G)) {
                cursorGrabbed = !cursorGrabbed;
                window.setMouseCursorGrabbed(cursorGrabbed);
                std::cout << "Cursor grabbed: " << cursorGrabbed << std::endl;
            }
        }
    }

    // Update the player's research progress. if the user clicked a tech tile on the tech tree.
   // unsigned int techId;
   // if (techTreeWindow.TryGetHitTechTile(&techId))
   // {
   //     bool switchedResearch = false;
   //     Player& currentPlayer = physicsSyncBuffer.LockPlayer(0);
   //     if (currentPlayer.SwitchResearch(techId))
   //     {
   //         switchedResearch = true;
   //     }
   //
   //     physicsSyncBuffer.UnlockPlayer(0);
   //
   //     // TODO play a sound if you fail to (or succeed in) switching research.
   // }
}

void Sim::Run() {
    // Create compatible OpenGL window
    std::ifstream f("Config/graphics.json");
    GraphicsConfig config = json::parse(f).template get<GraphicsConfig>();
    sf::ContextSettings windowSettings(config.depthBits, config.stencilBits, config.antialiasingLevel,
        config.openGlMajor, config.openGlMinor,
        debugCoreOpenGl ? sf::ContextSettings::Attribute::Core : sf::ContextSettings::Attribute::Default);
    sf::RenderWindow window(sf::VideoMode(config.width, config.height), "Sim", sf::Style::Default, windowSettings);
    window.setVerticalSyncEnabled(true);
    window.setActive(true);
    window.setMouseCursorGrabbed(true);
    window.setMouseCursorVisible(true);

    shaderFactory->InitCore();
    if (!testScene->Init(shaderFactory)) {
        Logger::LogError("Failed to load the test scene");
        return;
    }

    sf::ContextSettings usedSettings = window.getSettings();

    std::cout << "D:" << usedSettings.depthBits << ". S:" << usedSettings.stencilBits << ". A: " << usedSettings.antialiasingLevel << 
        ". GL: " << usedSettings.majorVersion << "." << usedSettings.minorVersion << std::endl;

    SimUpdateState state = SimUpdateState();

    bool isPaused = false;
    while (window.isOpen()) {
        Time::GlobalTime->Update(isPaused);

        HandleEvents(window, state);
        if (state.IsEscapePaused()) { // .ShouldQuit()) {
            window.close();
            break;
        }

        Update();
        Render(window);
        window.display();

        if (state.IsCaptureRequested()) {
            ScreenshotTaker::Take(window);
        }
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

//void TemperFine::PerformGuiThreadUpdates(float currentGameTime)
//{
//    // Update the selected voxel.
//   //vec::vec3i selectedVoxel;
//   //if (physicsSyncBuffer.TryGetNewSelectedVoxel(&selectedVoxel))
//   //{
//   //    voxelMap.SetSelectedVoxel(selectedVoxel);
//   //}
//   //
//   //// Update the map from changes, if applicable.
//   //if (physicsSyncBuffer.UpdateRoundMapDisplay(voxelMap))
//   //{
//   //    Logger::Log("Voxel Map Display updated!");
//   //}
//   //
//   //// Update the progress of the current research and stored resources
//   //int currentlyResearchingTech;
//   //float currentTechProgress = 0.0f;
//   //float fuelAmount, researchAmount;
//   //
//   //Player& currentPlayer = physicsSyncBuffer.LockPlayer(0);
//   //currentlyResearchingTech = currentPlayer.GetResearchProgress(&currentTechProgress);
//   //currentPlayer.GetStoredResources(&researchAmount, &fuelAmount);
//   //physicsSyncBuffer.UnlockPlayer(0);
//   //
//   //techProgressWindow.UpdateResearchProgress(currentlyResearchingTech, currentTechProgress);
//   //resourcesWindow.UpdateStoredResources(researchAmount, fuelAmount);
//   //
//   //// Update useful statistics that are fancier than the standard GUI
//   //statistics.UpdateRunTime(currentGameTime);
//   //statistics.UpdateViewPos(physicsSyncBuffer.GetViewerPosition());
//}