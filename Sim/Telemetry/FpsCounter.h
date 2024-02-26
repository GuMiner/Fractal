#pragma once

class FpsCounter {
    float timeTotal;
    int frameTotal;

    float lastFrameRateToRender;

public:
    FpsCounter();
    void Update();
    void Render();
};

