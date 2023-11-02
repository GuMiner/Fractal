#include <chrono>
#include <sstream>
#include "ScreenshotTaker.h"

using namespace std::chrono;

void ScreenshotTaker::Take(sf::RenderWindow& window) {
    // Replacement to RenderWindow's capture
    // https://www.sfml-dev.org/documentation/2.6.0/classsf_1_1RenderWindow.php
    sf::Vector2u windowSize = window.getSize();
    sf::Texture texture;
    texture.create(windowSize.x, windowSize.y);
    texture.update(window);
    sf::Image screenshot = texture.copyToImage();

    // Figure out the date to create an appropriate screenshot
    system_clock::time_point now = system_clock::now();
    time_t timeT = system_clock::to_time_t(now);
    tm* localTime = localtime(&timeT);

    std::stringstream screenshotPath;
    int monthOfDay = localTime->tm_mday;
    bool addZero = monthOfDay < 10;
    screenshotPath << "../Samples/";
    screenshotPath << localTime->tm_mon << "-" <<
        (addZero ? "0" : "") << localTime->tm_mday <<  "-" <<
        (localTime->tm_year - 100) << ".png";

    screenshot.saveToFile(screenshotPath.str());
}
