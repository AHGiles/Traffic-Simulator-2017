#include "Camera.h"

Camera::Camera(glm::vec3 pos, glm::mat4* projectionMatrix)
{
	this->projectionMatrix = projectionMatrix;
	this->pos = pos;								// Position of camera
	this->lookat = glm::vec3(0.0f, -1.0f, 0.0f);	// Lookat vector of camera
	this->up = glm::vec3(0.0f, 0.0f, -1.0f);		// Upwards vector of camera

	//Movement vectors
	this->forward = glm::vec3(0, 0, -1);
	this->right = glm::vec3(1, 0, 0);
	//Movement values
	this->rotation = 0;
	this->zoom = 0;
}

void Camera::changeCam(glm::vec3 pos, glm::vec3 lookat, glm::vec3 up) { 
	this->pos = pos; this->lookat = lookat; this->up = up;
	this->forward = glm::vec3(0, 0, -1);
	this->right = glm::vec3(1, 0, 0);
}

glm::mat4 Camera::getView() {
	return glm::lookAt(this->pos, this->pos + this->lookat, this->up);
}

glm::mat4 Camera::getProjectionMatrix(){
	return *this->projectionMatrix;
}


void Camera::moveCam(glm::vec2 moveDir, double deltaT) {
	switch ((int)moveDir.x) {
	case -1:
		this->pos = this->pos + (this->right * -(float)(moveValue * deltaT));
		break;
	case 1:
		this->pos = this->pos + (this->right * (float)(moveValue * deltaT));
	}

	switch ((int)moveDir.y) {
	case -1:
		this->pos = this->pos + (this->forward * -(float)(moveValue * deltaT));
		break;
	case 1:
		this->pos = this->pos + (this->forward * (float)(moveValue * deltaT));
	}
}

void Camera::rotateCam(int rotDir, double deltaT) {
	//std::cout << rotDir << std::endl;
	glm::mat4 rotMat = glm::mat4(1.0);
	switch (rotDir) {
	case -1:
		rotMat =  glm::rotate(glm::mat4(1.0), (float)(this->rotation - (rotValue * deltaT)), glm::vec3(0, 1, 0));
		this->lookat = glm::vec3(glm::vec4(this->lookat, 1.0) * rotMat);
		this->up = glm::vec3(glm::vec4(this->up, 1.0) * rotMat);
		this->forward = glm::vec3(glm::vec4(this->forward, 1.0) * rotMat);
		this->right = glm::vec3(glm::vec4(this->right, 1.0) * rotMat);
		break;
	case 1:
		rotMat = glm::rotate(glm::mat4(1.0), (float)(this->rotation + (rotValue * deltaT)), glm::vec3(0, 1, 0));
		this->lookat = glm::vec3(glm::vec4(this->lookat, 1.0) * rotMat);
		this->up = glm::vec3(glm::vec4(this->up, 1.0) * rotMat);
		this->forward = glm::vec3(glm::vec4(this->forward, 1.0) * rotMat);
		this->right = glm::vec3(glm::vec4(this->right, 1.0) * rotMat);
		break;
	}
}

void Camera::zoomCam(int zoomDir, double deltaT) {

	switch (zoomDir) {
	case -1:
		this->pos = this->pos + (this->lookat * -(float)(zoomValue * deltaT));
		break;
	case 1:
		this->pos = this->pos + (this->lookat * (float)(zoomValue * deltaT));
		break;
	}

}

glm::vec3 Camera::getPos(){
	return this->pos;
}