#pragma once
#include <SFML\System.hpp>
#include <SFML\Graphics.hpp>

class FpsCounter {
    float timeTotal;
    int frameTotal;

    float lastFrameRateToRender;

public:
    FpsCounter();
    void Update();
    void Render();
};

