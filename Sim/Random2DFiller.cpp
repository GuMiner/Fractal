#include "Random2DFiller.h"

Random2DFiller::Random2DFiller(uint32_t seed) : seed(seed), rgbRange(0, 255) {
	randGenerator.seed(seed);
}


void Random2DFiller::Fill(int yMin, int yMax, int width, unsigned char* rgbaData)
{
	for (int y = yMin; y < yMax; y++)
	{
		for (int x = 0; x < width; x++)
		{
			int pixelIdx = (x + y * width) * 4;
			rgbaData[pixelIdx] = rgbRange(randGenerator);
			rgbaData[pixelIdx + 1] = rgbRange(randGenerator);
			rgbaData[pixelIdx + 2] = rgbRange(randGenerator);
		}
	}
}
