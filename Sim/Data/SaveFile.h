#pragma once
#include "Config/SaveConfig.h"

class SaveFile
{
public:
	SaveConfig Config;
	SaveFile();
	void Save();
};

