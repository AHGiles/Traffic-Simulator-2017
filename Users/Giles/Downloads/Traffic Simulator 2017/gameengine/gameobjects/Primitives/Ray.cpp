#include "Ray.h"

Ray::Ray(float mouseX, float mouseY, float width, float height, Camera* cam){

	this->mouseX = mouseX;
	this->mouseY = mouseY;
	this->width = width;
	this->height = height;
	this->cam = cam;

}


glm::vec3 Ray::getRayDir()
{
	this->rayDir = glm::vec3(0, 0, 0);

	glm::vec4 viewport(0.0, 0.0, width, height);

	glm::vec3 rayOrigin2D = glm::vec3(mouseX, mouseY, 0);
	glm::vec3 rayEnd2D = glm::vec3(mouseX, mouseY, 1);

	glm::vec3 rayOrigin = glm::unProject(rayOrigin2D, cam->getView(), cam->getProjectionMatrix(), viewport);
	glm::vec3 rayEnd = glm::unProject(rayEnd2D, cam->getView(), cam->getProjectionMatrix(), viewport);

	if (rayEnd - rayOrigin != glm::vec3(0,0,0)) {
		glm::vec3 rayDirection = glm::normalize(rayEnd - rayOrigin);
		this->rayDir = rayDirection;
	}
	return this->rayDir;
}