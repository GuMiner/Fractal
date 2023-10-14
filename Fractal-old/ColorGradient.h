#pragma once
#include <vector>
#include <glm\vec3.hpp>

class ColorGradient
{
public:
    static std::vector<glm::vec3> GetColorGradient(int steps, float saturation, float value);
};

