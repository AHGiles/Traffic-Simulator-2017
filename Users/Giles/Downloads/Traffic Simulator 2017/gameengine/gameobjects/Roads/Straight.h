#pragma once
#include "Road.h"

class Straight : public Road {

public:

	Straight(glm::vec2 cellPos, glm::vec3 worldPos, AssimpModel** model);

	void Update(double deltaT);

	~Straight();

	void rotate();

};
