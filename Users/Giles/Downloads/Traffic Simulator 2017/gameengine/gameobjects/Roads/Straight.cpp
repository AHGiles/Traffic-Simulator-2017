#include "Straight.h"

Straight::Straight(glm::vec2 cellPos, glm::vec3 worldPos, AssimpModel** model) {

	//this->mesh = mesh;
	//this->tex = tex;
	this->model = model;

	this->cellPos = cellPos;
	this->worldPos = worldPos;

	this->transform = Transform();
	this->transform.setPos(this->worldPos);

	this->roadType = RoadType::STRAIGHT;

	this->roadLinks.push_back(new RoadDetails(Direction::NORTH, this->cellPos, this->worldPos));
	this->roadLinks.push_back(new RoadDetails(Direction::SOUTH, this->cellPos, this->worldPos));

	this->isSelected = 0;
	this->timeOccupiedByCar = 0;
	this->timesTraversed = 0;
}

void Straight::Update(double deltaT) {

	this->isSelected = 0;
}

void Straight::rotate() {

	for (RoadDetails* details : this->roadLinks) {
		details->rotate();
	}

	int newy = this->transform.getRot().y + 90;
	this->transform.setRot(glm::vec3(0, newy, 0));

}
