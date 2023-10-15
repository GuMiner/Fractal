#pragma once
#include <SFML\System.hpp>
#include <SFML\Graphics.hpp>

class FpsCounter {
    float lastTime;

    float timeTotal;
    int frameTotal;

    sf::Font* fpsFont;
    sf::Text* fpsText;

public:
    FpsCounter();
    void Update(float currentTime);
    void Render(sf::RenderWindow& window);
    ~FpsCounter();
};

