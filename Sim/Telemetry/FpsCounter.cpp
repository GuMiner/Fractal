#include <iostream>
#include <sstream>
#include "../Time.h"
#include "FpsCounter.h"

FpsCounter::FpsCounter() :
    timeTotal(0.1f), frameTotal(1)
{
    fpsFont = new sf::Font();
    if (!fpsFont->loadFromFile("Config/Fonts/DejaVuSans.ttf"))
    {
        std::cout << "ERROR: Could not load the font!" << std::endl;
    }

    fpsText = new sf::Text();
    fpsText->setFont(*fpsFont);
    fpsText->setCharacterSize(24);
    fpsText->setFillColor(sf::Color::Yellow); // sf::Color(128, 128, 128));
    fpsText->setString("FPS: ");
}

void FpsCounter::Update() {
    ++frameTotal;
    timeTotal += Time::GlobalTime->LastFrameInterval();

    // Only update every half second
    if (timeTotal > 0.50f)
    {
        float lastFrameRate = (float)frameTotal / (float)timeTotal;
        frameTotal = 0;
        timeTotal = 0.0f;

        std::stringstream fpsString;
        fpsString.precision(2);
        fpsString << std::fixed << "FPS: " << lastFrameRate;
        fpsText->setString(fpsString.str());
    }
}


void FpsCounter::Render(sf::RenderWindow& window) {
    window.draw(*fpsText);
}

FpsCounter::~FpsCounter() {
    delete fpsText;
    delete fpsFont;
}