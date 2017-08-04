#pragma once

#include "Road.h"

class Crossroad : public Road {
private:
	float maxGreenTime;
	float timeSinceGreen;

public:

	Crossroad(glm::vec2 cellPos, glm::vec3 worldPos, AssimpModel** model);

	void Update(double deltaT);

	void rotate();
};
