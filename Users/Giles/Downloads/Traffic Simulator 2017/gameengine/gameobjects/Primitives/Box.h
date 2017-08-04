#pragma once
#include "glm\glm.hpp"

struct Box
{
public:
	Box()
	{
		this->minX = 0;this->minY = 0;this->maxX = 0;this->maxY = 0;this->minZ = 0;this->maxZ = 0;
		min = glm::vec4(0, 0, 0,1); max = glm::vec4(0, 0, 0,1);
	}
	Box(float minx, float miny, float minz, float maxx, float maxy, float maxz)
	{
		this->minX = minx;this->minY = miny;this->maxX = maxx;this->maxY = maxy;this->minZ = minz;this->maxZ = maxz;
		min = glm::vec4(minx, miny, minz,1); max = glm::vec4(maxx, maxy, maxz,1);
	}

	float minX, minY, maxX, maxY, minZ, maxZ;
	glm::vec4 min, max;



};
