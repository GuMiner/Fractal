#include "TerrainTriangulator.h"
#include "GamePreprocessor.h"

GamePreprocessor::GamePreprocessor() {

}

bool GamePreprocessor::Process() {
	TerrainTriangulator triangulator;
	triangulator.TriangulateTerrain();
	return true;
}