#include <chrono>
#include <sstream>
#include <stb_image_write.h>
#include "ScreenshotTaker.h"

using namespace std::chrono;

void ScreenshotTaker::Take(GLFWwindow* window) {
    // Capture image
    int width, height;
    glfwGetWindowSize(window, &width, &height);

    unsigned char* image = new unsigned char[width * height * 4];
    glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, image);

    // Figure out the date to create an appropriate screenshot
    system_clock::time_point now = system_clock::now();
    time_t timeT = system_clock::to_time_t(now);
    tm* localTime = localtime(&timeT);

    std::stringstream screenshotPath;
    int monthOfDay = localTime->tm_mday;
    bool addZero = monthOfDay < 10;
    screenshotPath << "../Samples/";
    screenshotPath << (localTime->tm_mon + 1) << "-" <<
        (addZero ? "0" : "") << localTime->tm_mday <<  "-" <<
        (localTime->tm_year - 100) << ".png";


    stbi_flip_vertically_on_write(true);
    stbi_write_png(screenshotPath.str().c_str(), width, height, 4, image, 0);
}
