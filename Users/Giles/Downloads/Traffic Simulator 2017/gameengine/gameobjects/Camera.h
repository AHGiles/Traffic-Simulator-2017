#pragma once
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <iostream>

#define moveValue 2.5f
#define rotValue 1.0f
#define zoomValue 50.0f

struct Camera
{
private:

	glm::mat4* projectionMatrix;
	glm::vec3 pos;
	glm::vec3 lookat;
	glm::vec3 up;

	float rotation;
	float zoom;

	glm::vec3 forward;
	glm::vec3 right;
	//glm::vec2 lastMouse;
	
public:
	Camera() {}

	Camera(glm::vec3 pos, glm::mat4* projectionMatrix);

	void changeCam(glm::vec3 pos, glm::vec3 lookat, glm::vec3 up);

	glm::mat4 getView();
	//inline glm::mat4 getViewMatrix() const { return this->perspectiveMatrix * glm::lookAt(this->pos, this->pos + this->forward, this->up); } // Process the view matrix
	glm::mat4 getProjectionMatrix();

	void moveCam(glm::vec2 moveDir, double deltaT);
	void rotateCam(int rotDir, double deltaT);
	void zoomCam(int zoomDir, double deltaT);


	glm::vec3 getPos();
	glm::vec3 getUp() { return this->up; }
	glm::vec3 getRight() { return this->right;}
	glm::vec3 getDirection() { return this->forward; }
	//glm::mat4 getCameraRotation();

	//void moveForward(float mov);
	//void moveBack(float mov);
	//void moveLeft(float mov);
	//void moveRight(float mov);
	//void yaw(float mov);
	//void pitch(float mov);

	void mouseCameraMovement(float windowWidth, float windowHeight, int mouseX, int mouseY);

};