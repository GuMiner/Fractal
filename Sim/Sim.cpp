#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <thread>
#include <GL/glew.h>
#include <SFML/OpenGL.hpp>
#include <SFML/Graphics.hpp>
// #include <vld.h> // Enable for memory debugging.
//#include "Telemetry/Logger.h"
//#include "TemperFine.h"
//#include "../version.h"
#include <nlohmann/json.hpp>
#include "Sim.h"
#include "Data/Config/Config.h"

using json = nlohmann::json;

void Sim::SetupDiagnostics() {
    std::ifstream f("Config/diagnostics.json");
    DiagnosticsConfig config = json::parse(f).template get<DiagnosticsConfig>();
    simTexture.create(config.width, config.height);
    simTexture.setRepeated(false);
    simTexture.setSmooth(false);

    simSprite.setTexture(simTexture);
}

Sim::Sim() : fpsCounter(nullptr), threadProcessor(nullptr), filler(nullptr) {
}

Sim::~Sim() {
    delete fpsCounter;
    delete threadProcessor;
    delete filler;
}

void Sim::Init() {
    SetupDiagnostics();
    fpsCounter = new FpsCounter();
    threadProcessor = new ThreadProcessor();
    filler = new Random2DFiller(12345);

    // TODO setup thread processor with some test operation.
}

void Sim::Update(float currentTime) {
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

    fpsCounter->Update(currentTime);
}

void Sim::Render(sf::RenderWindow& window) {
    window.clear(sf::Color::Black);

    window.draw(simSprite);
    fpsCounter->Render(window);
}

void Sim::Run() {
    std::ifstream f("Config/graphics.json");
    GraphicsConfig config = json::parse(f).template get<GraphicsConfig>();
    sf::RenderWindow window(sf::VideoMode(config.width, config.height), "Sim");

    sf::Clock clock;
    while (window.isOpen())
    {
        float currentTime = clock.getElapsedTime().asSeconds();

        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }

        Update(currentTime);
        Render(window);
        window.display();
    }
}

int main()
{
    Sim sim;
    sim.Init();
    sim.Run();

    return 0;
}

//// Static definitions.
//Constants TemperFine::Constant;
//MathOps TemperFine::MathOp;
//PhysicsOps TemperFine::PhysicsOp;
//
//TemperFine::TemperFine()
//    : graphicsConfig("config/graphics.txt"), keyBindingConfig("config/keyBindings.txt"), physicsConfig("config/physics.txt"),
//      imageManager(), modelManager(&imageManager), techConfig("config/technologies.txt"),
//      armorConfig(&modelManager, "config/armors.txt"), bodyConfig(&modelManager, "config/bodies.txt"), turretConfig(&modelManager, "config/turrets.txt"),
//      physics(), scenery(&modelManager), physicsThread(&Physics::Run, &physics)
//{
//}
//
//void TemperFine::LogGraphicsSettings()
//{
//    Logger::Log("OpenGL vendor: ", glGetString(GL_VENDOR), ", version ", glGetString(GL_VERSION), ", renderer ", glGetString(GL_RENDERER));
//    Logger::Log("OpenGL extensions: ", glGetString(GL_EXTENSIONS));
//
//    GLint maxTextureUnits, maxUniformBlockSize;
//    GLint maxVertexUniformBlocks, maxFragmentUniformBlocks;
//    glGetIntegerv(GL_MAX_TEXTURE_UNITS, &maxTextureUnits);
//    glGetIntegerv(GL_MAX_VERTEX_UNIFORM_BLOCKS, &maxVertexUniformBlocks);
//    glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_BLOCKS, &maxFragmentUniformBlocks);
//    glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &maxUniformBlockSize);
//
//    Logger::Log("Max Texture Units: ", ", Max Uniform Size: ", (maxUniformBlockSize/1024), " kB");
//    Logger::Log("Max Vertex Uniform Blocks: ", maxVertexUniformBlocks, ", Max Fragment Uniform Blocks: ", maxFragmentUniformBlocks);
//}
//
//void TemperFine::UpdatePerspective(unsigned int width, unsigned int height)
//{
//    // Letterboxing is done at the top and bottom.
//    float necessaryWidth = (float)height * Constants::ASPECT;
//    if (necessaryWidth > width)
//    {
//        // Letterbox the top and the bottom of the screen so that the aspect ratio is met
//        float effectiveHeight = (float)width / Constants::ASPECT;
//        float heightDelta = ((float)height - effectiveHeight) / 2.0f;
//        glViewport(0, (int)heightDelta, (GLsizei)width, (GLsizei)effectiveHeight);
//    }
//    else
//    {
//        // Letterbox the left and the right so that the aspect ratio is met.
//        float widthDelta = ((float)width - necessaryWidth) / 2.0f;
//        glViewport((GLint)widthDelta, (GLint)0, (GLsizei)necessaryWidth, (GLsizei)height);
//    }
//}
//
//Constants::Status TemperFine::LoadGraphics(sfg::Desktop* desktop)
//{
//    // Setup GLEW
//    GLenum err = glewInit();
//    if (err != GLEW_OK)
//    {
//        Logger::LogError("GLEW startup failure: ", err, ".");
//        return Constants::Status::BAD_GLEW;
//    }
//
//    // Log graphics information for future reference
//    LogGraphicsSettings();
//
//    // Enable alpha blending
//    glEnable(GL_BLEND);
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//
//    // Enable line and polygon smoothing
//    glEnable(GL_LINE_SMOOTH);
//    glEnable(GL_POLYGON_SMOOTH);
//
//    // Multisample if available
//    glEnable(GL_MULTISAMPLE);
//
//    // Let OpenGL shaders determine point sizes.
//    glEnable(GL_PROGRAM_POINT_SIZE);
//
//    // Disable face culling so that see-through flat objects work.
//    glDisable(GL_CULL_FACE);
//    glFrontFace(GL_CW);
//
//    // Cutout faces that are hidden by other faces.
//    glEnable(GL_DEPTH_TEST);
//    glDepthFunc(GL_LEQUAL);
//
//    // Assets
//    if (!desktop->LoadThemeFromFile("themes/temperfine.theme"))
//    {
//        Logger::LogError("Could not load the GUI theme file!");
//        return Constants::Status::BAD_THEME;
//    }
//
//    return LoadAssets(desktop);
//}
//

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
//
//void TemperFine::HandleEvents(sfg::Desktop& desktop, sf::RenderWindow& window, bool& alive, bool& focusPaused, bool& escapePaused)
//{
//    // Handle all events.
//    sf::Event event;
//    while (window.pollEvent(event))
//    {
//        // Update SF GUI. NOTE: This runs on the GUI thread!
//        desktop.HandleEvent(event);
//
//        if (event.type == sf::Event::Closed)
//        {
//            alive = false;
//        }
//        else if (event.type == sf::Event::LostFocus)
//        {
//            focusPaused = true;
//        }
//        else if (event.type == sf::Event::GainedFocus)
//        {
//            focusPaused = false;
//        }
//        else if (event.type == sf::Event::Resized)
//        {
//            UpdatePerspective(event.size.width, event.size.height);
//        }
//        else if (event.type == sf::Event::KeyReleased)
//        {
//            if (event.key.code == KeyBindingConfig::ToggleTechTreeWindow)
//            {
//                techTreeWindow.ToggleDisplay();
//            }
//            else if (event.key.code == KeyBindingConfig::ToggleTechProgressWindow)
//            {
//                techProgressWindow.ToggleDisplay();
//            }
//            else if (event.key.code == KeyBindingConfig::ToggleResourcesWindow)
//            {
//                resourcesWindow.ToggleDisplay();
//            }
//            else if (event.key.code == KeyBindingConfig::ToggleBuildingsWindow)
//            {
//                buildingsWindow.ToggleDisplay();
//            }
//            else if (event.key.code == sf::Keyboard::Escape)
//            {
//                // The Escape key is the consistent pause / unpause
//                escapePaused = !escapePaused;
//                escapeConfigWindow.ToggleDisplay();
//            }
//        }
//        else if (event.type == sf::Event::MouseButtonPressed)
//        {
//            if (event.mouseButton.button == sf::Mouse::Left && 
//                !techTreeWindow.WithinVisibleBounds(event.mouseButton.x, event.mouseButton.y) &&
//                !buildingsWindow.WithinVisibleBounds(event.mouseButton.x, event.mouseButton.y) &&
//                !escapeConfigWindow.WithinVisibleBounds(event.mouseButton.x, event.mouseButton.y))
//            {
//                physics.QueueLeftMouseClick(event.mouseButton.x, event.mouseButton.y, window.getSize().x, window.getSize().y);
//            }
//        }
//    }
//
//    // Update the player's research progress. if the user clicked a tech tile on the tech tree.
//    unsigned int techId;
//    if (techTreeWindow.TryGetHitTechTile(&techId))
//    {
//        bool switchedResearch = false;
//        Player& currentPlayer = physicsSyncBuffer.LockPlayer(0);
//        if (currentPlayer.SwitchResearch(techId))
//        {
//            switchedResearch = true;
//        }
//
//        physicsSyncBuffer.UnlockPlayer(0);
//
//        // TODO play a sound if you fail to (or succeed in) switching research.
//    }
//
//    // Update windows based on size resizing.
//    resourcesWindow.MoveToScreenBottom(window.getSize());
//    techProgressWindow.MoveToScreenBottomLeft(window.getSize());
//
//    // Handle pausing the physics thread if we're paused.
//    if (escapePaused || focusPaused)
//    {
//        physics.Pause();
//    }
//    else
//    {
//        physics.Resume();
//    }
//
//    // Handle alternative means of quitting.
//    if (escapeConfigWindow.ShouldQuit())
//    {
//        alive = false;
//    }
//}
//
//Constants::Status TemperFine::Run()
//{
//    // 24 depth bits, 8 stencil bits, 8x AA, major version 4.
//    Logger::Log("Graphics Initializing...");
//    //sf::ContextSettings contextSettings = sf::ContextSettings(24, 8, 8, 4, 0);
//
//   // sf::Uint32 style = GraphicsConfig::IsFullscreen ? sf::Style::Fullscreen : sf::Style::Titlebar | sf::Style::Resize | sf::Style::Close;
//   // sf::RenderWindow window(sf::VideoMode(GraphicsConfig::ScreenWidth, GraphicsConfig::ScreenHeight), "Temper Fine", style, contextSettings);
//    sf::RenderWindow window(sf::VideoMode(800, 600), "My window");
//    sfg::Desktop desktop;
//
//    // Now that we have an OpenGL Context, load our graphics.
//    Constants::Status firstTimeSetup = LoadGraphics(&desktop);
//    if (firstTimeSetup != Constants::Status::OK)
//    {
//        return firstTimeSetup;
//    }
//
//    Logger::Log("Graphics Initialized!");
//
//    sf::Clock clock;
//    sf::Clock guiClock;
//    sf::Time clockStartTime;
//    bool alive = true;
//    bool focusPaused = false;
//    bool escapePaused = false;
//    vec::mat4 viewMatrix;
//    while (alive)
//    {
//        clockStartTime = clock.getElapsedTime();
//        viewMatrix = physicsSyncBuffer.GetViewMatrix();
//
//        HandleEvents(desktop, window, alive, focusPaused, escapePaused);
//        PerformGuiThreadUpdates(clock.getElapsedTime().asSeconds());
//
//        // Render, only if non-paused.
//        if (!focusPaused && !escapePaused)
//        {
//            Render(desktop, window, viewMatrix);
//        }
//
//        // GUI is always updated, as else when we're paused, we can't see it!
//        
//        // Renders the UI, unbinding the current vertex array to avoid messiness as SFGUI also uses OpenGL, of course.
//        glBindVertexArray(0);
//        glUseProgram(0);
//        glViewport(0, 0, window.getSize().x, window.getSize().y);
//        desktop.Update(guiClock.restart().asSeconds());
//        sfgui.Display(window);
//
//        // Display what we rendered.
//        UpdatePerspective(window.getSize().x, window.getSize().y);
//        window.display();
//
//        // Delay to run approximately at our maximum framerate.
//        sf::Int64 sleepDelay = (1000000 / Constants::MAX_FRAMERATE) - clock.getElapsedTime().asMicroseconds() - clockStartTime.asMicroseconds();
//        sf::sleep(sf::microseconds(sleepDelay));
//    }
//
//    return Constants::Status::OK;
//}
//
//void TemperFine::Deinitialize()
//{
//    // TODO Test code remove.
//    mapManager.ClearMap(testMap);
//
//    Logger::Log("Music Thread Stopping...");
//    //musicManager.Stop();
//
//    Logger::Log("Physica Thread Stopping...");
//    physics.Stop();
//    physicsThread.wait();
//    Logger::Log("Physics Thread Stopped.");
//
//   // musicThread.wait();
//    Logger::Log("Music Thread Stopped.");
//}
//
//// Runs the main application.
//int main(int argc, char* argv[])
//{
//    std::cout << "Sim Start!" << std::endl;
//
//    // Startup 'static' stuff
//    TemperFine::Constant = Constants();
//    TemperFine::MathOp = MathOps();
//    TemperFine::PhysicsOp = PhysicsOps();
//
//    Logger::Setup();
//
//    Logger::Log("TemperFine ", AutoVersion::MAJOR_VERSION, ".", AutoVersion::MINOR_VERSION, ".");
//
//    Constants::Status runStatus;
//    std::unique_ptr<TemperFine> temperFine(new TemperFine());
//
//    // Run the application.
//    runStatus = temperFine->Initialize();
//    if (runStatus == Constants::Status::OK)
//    {
//        runStatus = temperFine->Run();
//        temperFine->Deinitialize();
//    }
//    else
//    {
//        Logger::LogError("Could not initialize Temper Fine!");
//    }
//
//    // Wait before closing for display purposes.
//    Logger::Log("Application End!");
//    Logger::Shutdown();
//    std::cout << "TemperFine End!" << std::endl;
//
//    sf::sleep(sf::milliseconds(1000));
//
//    // Log is auto-shutdown.
//    return (int)runStatus;
//}