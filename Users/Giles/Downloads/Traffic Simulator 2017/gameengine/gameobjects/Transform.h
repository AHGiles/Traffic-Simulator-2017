#pragma once
#include <glm\glm.hpp>
#include <glm/gtx/transform.hpp>
#include "glm\gtc\matrix_inverse.hpp"

struct Transform
{
public:

	Transform()
	{
		this->position = glm::vec3();
		this->rotation = glm::vec3();
		this->scale = glm::vec3(1.0f,1.0f,1.0f);
	}

	Transform(const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& sca)
	{
		this->position = pos;
		this->rotation = rot;
		this->scale = sca;
	}

	inline glm::mat4 getModel() const // Compute the model matrix
	{
		glm::mat4 posMat = glm::translate(position); // Translation
		glm::mat4 scaleMat = glm::scale(scale); // Scale
		glm::mat4 rotX = glm::rotate(rotation.x, glm::vec3(1.0, 0.0, 0.0)); // X Rotation
		glm::mat4 rotY = glm::rotate(rotation.y, glm::vec3(0.0, 1.0, 0.0)); // Y Rotation
		glm::mat4 rotZ = glm::rotate(rotation.z, glm::vec3(0.0, 0.0, 1.0)); // Z Rotation
		glm::mat4 rotMat = rotX * rotY * rotZ; // Compute all rotations together
		glm::mat4 retVal = posMat*rotMat*scaleMat; // Compute all translations together
		return retVal; // Return the model matrix
	}

	inline glm::mat3 getNormal() const // Compute the normal Matrix
	{
		glm::mat3 Normal = glm::inverseTranspose(glm::mat3(this->getModel()));
		return Normal;
	}

	// Getters and Setters
	inline glm::vec3& getPos() { return position; }
	inline glm::vec3& getRot() { return rotation; }
	inline glm::vec3& getScale() { return scale; }

	void setPos(glm::vec3& pos) { position = pos; }
	void setRot(glm::vec3& rot) { rotation = rot; }
	void setScale(glm::vec3& sca) { scale = sca; }

	
private:
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
protected:

};

