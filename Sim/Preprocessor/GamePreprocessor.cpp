#include "TerrainTriangulator.h"
#include "GamePreprocessor.h"

GamePreprocessor::GamePreprocessor() {

}

bool GamePreprocessor::Process() {
	TerrainTriangulator triangulator;
	// All done! triangulator.GenerateMipMaps();
	triangulator.TriangulateTerrain();
	return true;
}