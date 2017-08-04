#pragma once

#include "Road.h"

class TJunc : public Road {
private:
	float maxGreenTime;
	float timeSinceGreen;

public:

	TJunc(glm::vec2 cellPos, glm::vec3 worldPos, AssimpModel** model);

	void Update(double deltaT);

	void rotate();
};
