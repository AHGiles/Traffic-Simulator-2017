#include "Car.h"


Car::Car(glm::vec2 cellPos, glm::vec3 worldPos, Direction dir, AssimpModel* model, int* gridSize, int* tileSize){

	this->cellPos = cellPos;
	this->worldPos = worldPos;
	this->direction = dir;
	this->vel = 25;
	//this->mesh = mesh;
	//this->tex = tex;
	this->model = model;

	this->transform = Transform();
	this->transform.setPos(this->worldPos);
	//this->transform.setScale(glm::vec3( 3, 3, 3));
	currentID++;
	this->id = currentID;

	this->gridSize = gridSize;
	this->tileSize = tileSize;
	timeStatic = 0;
	moveVec = glm::vec3();
}

void Car::Init(int* roadMap, vector<Direction>** carMap){

	generateNewPath(roadMap, carMap);
	//path.pop_back();//remove first cell of path as this is the current cell.

}

void Car::Draw(Shader* carShader, Camera cam){

	carShader->Bind();

	this->model->BindModelsVAO();

	//roadShader->UpdateLight(this->transform, cam, &mat_Shininess);
	carShader->Update(this->transform, cam);

	// Bind texture
	//this->tex->Bind(0);

	// Enable textures
	glEnable(GL_TEXTURE_2D);
	//this->mesh->Draw();
	this->model->RenderModel();

}

void Car::Update(int * roadMap, Road*** roads, vector<Direction>** carMap, double deltaT){
	
	if (timeStatic > 15) {
		generateNewPath(roadMap, carMap);
		timeStatic = 0;
	}
	//process the current path
	processPath();

	//if path is now empty, make new path
	if (path.size() == 0)
		generateNewPath(roadMap, carMap);

	//calculate the movement based on path and road map
	moveVec = calcMovement(roads, carMap, deltaT);

	//move the car given the vector previously determined
	moveCar(moveVec, carMap, roads);
	
}

void Car::processPath(){

	//get current cell
	cellPos.x = (int)worldPos.x / *tileSize;
	cellPos.y = (int)worldPos.z / *tileSize;

	//if current cell is equal to back of the path list, remove from path
	if (path.back().x == cellPos.x && path.back().y == cellPos.y) {
		path.pop_back();
	}


}

void Car::generateNewPath(int* roadMap, vector<Direction>** carMap){

	AStar pathfinder(roadMap, false, this->direction,*gridSize ,carMap);
	srand(time(NULL) + id);
	int x = rand() % *gridSize;
	int y = rand() % *gridSize;
	while (roadMap[y * *gridSize + x] == 0 || roadMap[y * *gridSize + x] > 6 || (x == cellPos.x && y == cellPos.y)) {
		x = rand() % *gridSize;
		y = rand() % *gridSize;
	}
	path = pathfinder.search(cellPos, glm::vec2(x, y));
	if (path.size() == 1) {
		int help = 1;
	}
	//path.pop_back();//remove first cell of path as this is the current cell.

}

glm::vec3 Car::calcMovement(Road*** roads, vector<Direction>** carMap, double deltaT){

	//process movement from current cell and world pos to next cell
	//move from current pos towards the entry point on next cell based on current direction.
	//if next cell is entered, update direction variable

	glm::vec2 next = path.back();
	std::pair <int, int> cur, nxt;
	cur.first = cellPos.x;
	cur.second = cellPos.y;
	nxt.first = next.x;
	nxt.second = next.y;

	Road* currentRoad = roads[cur.first][cur.second];
	Road* nextRoad = roads[nxt.first][nxt.second];

	//move on current road piece, towards next road piece
	//work out entry direction into next cell
	int diffX = nxt.first - cur.first;
	int diffY = nxt.second - cur.second;
	Direction entryDir;
	if (diffX != 0) {
		if (diffX == 1)
			entryDir = EAST;
		else
			entryDir = WEST;
	}
	else {
		if (diffY == 1)
			entryDir = SOUTH;
		else
			entryDir = NORTH;
	}
	//get target location from enty point in next cell
	std::pair <glm::vec3, glm::vec3> targets;
	glm::vec3 targetPos = glm::vec3(1.0);
	for each (RoadDetails* r in nextRoad->roadLinks) {
		if (r->dir == entryDir) {
			targetPos = r->worldPosIn;
		}
	}
	//ignore any error in the y axis
	targetPos.y = 0;


	switch (direction) {
	case NORTH:
		targets.first = glm::vec3(worldPos.x, worldPos.y, targetPos.z);
		break;
	case EAST:
		targets.first = glm::vec3(targetPos.x, worldPos.y, worldPos.z);
		break;
	case SOUTH:
		targets.first = glm::vec3(worldPos.x, worldPos.y, targetPos.z);
		break;
	case WEST:
		targets.first = glm::vec3(targetPos.x, worldPos.y, worldPos.z);
		break;
	}
	
	targets.second = targetPos;

	float dist = glm::length(worldPos - targets.first);
	if (dist < 5) {
		std::swap(targets.first,targets.second);
	}
	//get movement vector for car
	glm::vec3 moveVec;
	if(targets.first != worldPos)
		moveVec = glm::normalize(targets.first - worldPos);
	else {
		switch (direction) {
		case NORTH:
			moveVec = glm::vec3(0, 0, -0.25);
			break;
		case EAST:
			moveVec = glm::vec3(0.25, 0, 0);
			break;
		case SOUTH:
			moveVec = glm::vec3(0, 0, 0.25);
			break;
		case WEST:
			moveVec = glm::vec3(-0.25, 0, 0);
			break;
		}
	}

	switch (direction) {
	case NORTH:
		(moveVec.x > 0) ? vel = 12 : vel = 25;
		break;
	case EAST:
		(moveVec.z > 0) ? vel = 12 : vel = 25;
		break;
	case SOUTH:
		(moveVec.x > 0) ? vel = 12 : vel = 25;
		break;
	case WEST:
		(moveVec.z > 0) ? vel = 12 : vel = 25;
		break;
	}

	//Method that handles traffic and junction control
	trafficJunctionControl(moveVec, targetPos, currentRoad, nextRoad, nxt, carMap, entryDir, deltaT, roads);

	return moveVec;
}

void Car::trafficJunctionControl(glm::vec3 moveVec, glm::vec3 targetPos, Road* currentRoad, Road* nextRoad, std::pair <int, int> nxt, vector<Direction>** carMap, Direction entryDir, double deltaT, Road*** roads){
	
	//Check to see if at end of cell
	if (glm::length(targetPos - worldPos) < 15 && currentRoad->roadType < 4) {
		bool notClear = false;

		//check for any  cars in the next cell
		if (carMap[nxt.first][nxt.second].size() > 0) {

			//if corner or straight road, check if current direction is occupied
			if (nextRoad->roadType == STRAIGHT || nextRoad->roadType == CORNER) {

				for each (Direction d in carMap[nxt.first][nxt.second]) {
					if (d == entryDir) {
						notClear = true;
						//nextRoad->isSelected = 1;
						continue;
					}
				}
			}

			else {
				notClear = true;
			}
		}

		//If next road is a junction then check priority
		//if (nextRoad->roadType == TJUNC){// || nextRoad->roadType == CROSSROADS) {
		//	if (path.size() >= 2) {
		//		int nnx, nny, diffX2, diffY2;
		//		nnx = path[path.size() - 2].x;
		//		nny = path[path.size() - 2].y;
		//		diffX2 = nnx - nxt.first;
		//		diffY2 = nny - nxt.second;
		//		Direction entryDir2;
		//		if (diffX2 != 0) {
		//			if (diffX2 == 1)
		//				entryDir2 = EAST;
		//			else
		//				entryDir2 = WEST;
		//		}
		//		else {
		//			if (diffY2 == 1)
		//				entryDir2 = SOUTH;
		//			else
		//				entryDir2 = NORTH;
		//		}
		//		for each (Direction d in carMap[nnx][nny]) {
		//			if (d == entryDir2) {
		//				notClear = true;
		//				continue;
		//			}
		//		}
		//	}
		//}

		//no car in the next cell
		else if (nextRoad->roadType == CROSSROADS || nextRoad->roadType == TJUNC) {

			//if not empty
			if (!nextRoad->activeTraffic.empty()) {

				//if crossroad isn't allowing this traffic through add to queue and wait
				if (nextRoad->activeTraffic.front() != entryDir) {
					std::deque<Direction> cont = nextRoad->activeTraffic._Get_container();
					bool match = false;
					for (int i = 1; i < cont.size(); i++) {
						if (cont[i] == entryDir)
							match = true;
					}
					if(!match)
						nextRoad->activeTraffic.push(entryDir);
					notClear = true;
				}
				//Else Check to see if exit is clear
				else {
					if (path.size() >= 2) {
						int nnx, nny, diffX2, diffY2;
						nnx = path[path.size() - 2].x;
						nny = path[path.size() - 2].y;
						diffX2 = nnx - nxt.first;
						diffY2 = nny - nxt.second;
						Direction entryDir2;
						if (diffX2 != 0) {
							if (diffX2 == 1)
								entryDir2 = EAST;
							else
								entryDir2 = WEST;
						}
						else {
							if (diffY2 == 1)
								entryDir2 = SOUTH;
							else
								entryDir2 = NORTH;
						}
						for each (Direction d in carMap[nnx][nny]) {
							if (d == entryDir2) {
								notClear = true;
								//roads[nnx][nny]->isSelected = 1;
								continue;
							}
						}
					}
				}
			}
			else {
				nextRoad->activeTraffic.push(entryDir);
			}


		}
		//only move if next cell is clear and priority is yours
		if (!notClear) {
			worldPos += (moveVec * (float)(vel * deltaT));//move car in required direction
			timeStatic = 0;
		}
		else
			timeStatic += deltaT;
	}

	//if not at end of cell keep moving
	else {
		worldPos += (moveVec * (float)(vel * deltaT));//move car in required direction
		timeStatic = 0;
	}

}

void Car::moveCar(glm::vec3 moveVec, vector<Direction>** carMap, Road*** roads){

	//set the transform of the car to the calculated world position
	this->transform.setPos(worldPos);

	//check if car has moved to new cell
	glm::vec2 newCellPos = glm::vec2((int)worldPos.x / *tileSize, (int)worldPos.z / *tileSize);
	if (newCellPos.x != cellPos.x || newCellPos.y != cellPos.y) {
		//if it has moved cell then remove presence in previous cell
		for (int i = 0; i < carMap[(int)cellPos.x][(int)cellPos.y].size(); i++) {
			if (carMap[(int)cellPos.x][(int)cellPos.y][i] == direction)
				carMap[(int)cellPos.x][(int)cellPos.y].erase(carMap[(int)cellPos.x][(int)cellPos.y].begin() + i);
		}
		//calculate new direction
		int cellDiffX = newCellPos.x - cellPos.x;
		int cellDiffY = newCellPos.y - cellPos.y;
		if (cellDiffX != 0) {
			if (cellDiffX == 1)
				this->direction = EAST;
			else
				this->direction = WEST;
		}
		else {
			if (cellDiffY == 1)
				this->direction = SOUTH;
			else
				this->direction = NORTH;
		}
		//add presence to new cell
		carMap[(int)newCellPos.x][(int)newCellPos.y].push_back(Direction(direction));
		roads[(int)newCellPos.x][(int)newCellPos.y]->timesTraversed++;
	}

	//set the rotation of the car to some voodoo calculation?
	float dot = glm::dot(glm::vec2(moveVec.x, moveVec.z), glm::vec2(0, 1));
	float det = moveVec.x;
	float angle = atan2f(det, dot);
	this->transform.setRot(glm::vec3(0, (angle * 180) / 3.141, 0));

}
