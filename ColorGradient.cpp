#include <glm\gtx\color_space.hpp>
#include "ColorGradient.h"

std::vector<glm::vec3> ColorGradient::GetColorGradient(int steps, float saturation, float value)
{
    std::vector<glm::vec3> gradient;
    for (int i = 0; i < steps; i++)
    {
        // Full HSV goes from 0 to 260. We stop at mid-purple.
        float hue = 320.0f * ((float)i / (float)steps);
        gradient.push_back(glm::rgbColor(glm::vec3(hue, saturation, value)));
    }

    return gradient;
}
