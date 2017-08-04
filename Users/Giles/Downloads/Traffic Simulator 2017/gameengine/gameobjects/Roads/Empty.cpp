#include "Empty.h"

Empty::Empty(glm::vec2 cellPos, glm::vec3 worldPos, AssimpModel** model){

	//this->mesh = mesh;
	//this->tex = tex;
	this->model = model;

	this->cellPos = cellPos;
	this->worldPos = worldPos;

	this->transform = Transform();
	this->transform.setPos(this->worldPos);

	this->roadType = RoadType::EMPTY;

	this->isSelected = 0;
	this->timesTraversed = 0;

}

void Empty::Update(double deltaT) {

	this->isSelected = 0;
}

void Empty::rotate() {
	//Empty road so do nothing
}