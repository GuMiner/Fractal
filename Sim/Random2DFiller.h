#pragma once
#include <random>
class Random2DFiller
{
	uint32_t seed;
	std::mt19937 randGenerator; // mersenne twister
	std::uniform_int_distribution<int> rgbRange;

public:
	Random2DFiller(uint32_t seed);
	void Fill(int yMin, int yMax, int width, unsigned char* rgbaData);
};

