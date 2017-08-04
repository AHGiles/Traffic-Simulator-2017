#pragma once

#include "Road.h"

class Corner : public Road {
public:

	Corner(glm::vec2 cellPos, glm::vec3 worldPos, AssimpModel** model);

	void Update(double deltaT);

	void rotate();
};