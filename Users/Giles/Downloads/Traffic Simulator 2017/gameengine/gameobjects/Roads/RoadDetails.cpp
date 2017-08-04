#include "RoadDetails.h"

RoadDetails::RoadDetails(Direction dir, glm::vec2 startCellPos, glm::vec3 startWorldPos) {

	this->dir = dir;

	switch (dir) {

	case(NORTH):
			this->cellPos = glm::vec2(startCellPos.x, (startCellPos.y + 1));
			this->worldPosIn = glm::vec3((startWorldPos.x - 10), startWorldPos.y, (startWorldPos.z + TILESIZE/2));
		break;

	case(EAST):
		this->cellPos = glm::vec2((startCellPos.x - 1), startCellPos.y);
		this->worldPosIn = glm::vec3((startWorldPos.x - TILESIZE/2), startWorldPos.y, (startWorldPos.z - 10));
		break;

	case(SOUTH):
		this->cellPos = glm::vec2(startCellPos.x, (startCellPos.y - 1));
		this->worldPosIn = glm::vec3((startWorldPos.x + 10), startWorldPos.y, (startWorldPos.z - TILESIZE/2));
		break;

	case(WEST):
		this->cellPos = glm::vec2((startCellPos.x + 1), startCellPos.y);
		this->worldPosIn = glm::vec3((startWorldPos.x + TILESIZE/2), startWorldPos.y, (startWorldPos.z + 10));
		break;

	}
	

}

void RoadDetails::rotate(){

	switch (this->dir) {
	case(NORTH):
		this->dir			= EAST;
		this->cellPos		= glm::vec2( (this->cellPos.x - 1), (this->cellPos.y + 1));
		this->worldPosIn	= glm::vec3( this->worldPosIn.x - 15, this->worldPosIn.y, this->worldPosIn.z - 35);
		break;
	case(EAST):
		this->dir			= SOUTH;
		this->cellPos		= glm::vec2( (this->cellPos.x + 1), (this->cellPos.y - 1));
		this->worldPosIn	= glm::vec3( this->worldPosIn.x + 35, this->worldPosIn.y, this->worldPosIn.z - 15);
		break;
	case(SOUTH):
		this->dir			= WEST;
		this->cellPos		= glm::vec2( (this->cellPos.x + 1), (this->cellPos.y + 1));
		this->worldPosIn	= glm::vec3( this->worldPosIn.x + 15, this->worldPosIn.y, this->worldPosIn.z + 35);
		break;
	case(WEST):
		this->dir = NORTH;
		this->cellPos		= glm::vec2( (this->cellPos.x - 1), (this->cellPos.y + 1));
		this->worldPosIn	= glm::vec3( this->worldPosIn.x - 35, this->worldPosIn.y, this->worldPosIn.z + 15);
		break;
	}
}


