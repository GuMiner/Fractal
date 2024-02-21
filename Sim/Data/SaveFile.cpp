#include <fstream>
#include <ostream>
#include "SaveFile.h"

SaveFile::SaveFile() {
	std::ifstream f("Config/save.json");

	// TODO consider error handling if the file does not exist.
	Config = json::parse(f).template get<SaveConfig>();
}

void SaveFile::Save() {
	std::ofstream f("Config/save.json");
	
	json j;
	to_json(j, Config);

	f << std::setw(2) << j << std::endl;
}