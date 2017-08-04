#include "TJunc.h"

TJunc::TJunc(glm::vec2 cellPos, glm::vec3 worldPos, AssimpModel** model) {

	//this->mesh = mesh;
	//this->tex = tex;
	this->model = model;

	this->cellPos = cellPos;
	this->worldPos = worldPos;

	this->transform = Transform();
	this->transform.setPos(this->worldPos);
	this->transform.setRot(glm::vec3(0, 0, 0));
	this->roadType = RoadType::TJUNC;

	this->roadLinks.push_back(new RoadDetails(Direction::NORTH, this->cellPos, this->worldPos));
	this->roadLinks.push_back(new RoadDetails(Direction::SOUTH, this->cellPos, this->worldPos));
	this->roadLinks.push_back(new RoadDetails(Direction::EAST, this->cellPos, this->worldPos));

	this->maxGreenTime = 10;
	this->timeSinceGreen = 0;

	this->isSelected = 0;
	this->timeOccupiedByCar = 0;
	this->timesTraversed = 0;
	
}

void TJunc::Update(double deltaT) {
	this->timeSinceGreen += deltaT;
	if (activeTraffic.size() > 1) {		
		if (timeSinceGreen >= maxGreenTime) {
			this->activeTraffic.pop();
			this->timeSinceGreen = 0;
		}
	}

	this->isSelected = 0;
}

void TJunc::rotate() {

	for (RoadDetails* details : this->roadLinks) {
		details->rotate();
	}

	int newy = this->transform.getRot().y - 90;
	this->transform.setRot(glm::vec3(0, newy, 0));
}