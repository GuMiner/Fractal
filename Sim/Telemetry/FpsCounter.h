#pragma once
#include <SFML\System.hpp>
#include <SFML\Graphics.hpp>

class FpsCounter {
    float timeTotal;
    int frameTotal;

    sf::Font* fpsFont;
    sf::Text* fpsText;

public:
    FpsCounter();
    void Update();
    void Render(sf::RenderWindow& window);
    ~FpsCounter();
};

