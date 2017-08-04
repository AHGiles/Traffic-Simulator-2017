#pragma once
#include "../include/glm/glm.hpp"

#include "../Primitives/Direction.h"

#define TILESIZE 50

class RoadDetails {

public:
	Direction dir;
	glm::vec2 cellPos;
	glm::vec3 worldPosIn;
//	glm::vec3 worldPosOut;

	RoadDetails(Direction dir, glm::vec2 startCellPos, glm::vec3 startWorldPos);
	void rotate();

};