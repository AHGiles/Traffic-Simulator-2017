#pragma once
#include "glm\glm.hpp"
#include <iostream>

#include "../Camera.h"

class Ray
{
private:
	float mouseX, mouseY;
	float width, height;
	Camera* cam;

	glm::vec3 rayDir;

public:
	Ray(){}
	Ray(float mouseX, float mouseY, float width, float height, Camera* cam);

	glm::vec3 getRayDir();

};