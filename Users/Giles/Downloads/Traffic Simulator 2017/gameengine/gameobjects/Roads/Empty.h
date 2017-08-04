#pragma once
#include "Road.h"

class Empty : public Road {
public:

	Empty(glm::vec2 cellPos, glm::vec3 worldPos, AssimpModel** model);

	void Update(double deltaT);

	void rotate();

};