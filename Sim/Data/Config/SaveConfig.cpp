#include "SaveConfig.h"

void to_json(json& j, const SaveConfig& c) {
	j = json{ {"position", c.position}, {"forwards", c.forwards}, {"up", c.up} };
}

void from_json(const json& j, SaveConfig& c) {
	ParseVector(j, "position", c.position);
	ParseVector(j, "forwards", c.forwards);
	ParseVector(j, "up", c.up);
}