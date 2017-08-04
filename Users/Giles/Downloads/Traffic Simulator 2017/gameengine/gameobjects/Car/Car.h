#pragma once

#include <time.h>
#include "../../AStar/AStar.h"
//#include "../Primitives/Mesh.h"
//#include "../../ObjectLoading/Texture.h"
#include "../Roads/Road.h"

enum Lane {
	CLEAR,
	CLOCKWISE,
	ANTICLOCKWISE,
	BOTH
};

static int currentID = 0;
class Car {
	
public:
	
	int id;
	//Mesh* mesh;
	//Texture* tex;
	AssimpModel* model;
	Transform transform;

	glm::vec3 worldPos;
	glm::vec2 cellPos;

	vector<glm::vec2> path;
	float vel;
	Direction direction;

	int* gridSize;
	int* tileSize;
	float timeStatic;
	glm::vec3 moveVec;

public:

	Car() {}

	Car(glm::vec2 cellPos, glm::vec3 worldPos, Direction dir, AssimpModel* model, int* gridSize, int* tileSize);

	void Init(int* roadMap, vector<Direction>** carMap);

	void Draw(Shader* carShader, Camera cam);

	void Update(int* roadMap, Road*** roads, vector<Direction>** carMap, double deltaT);

	void processPath();

	void generateNewPath(int* roadMap, vector<Direction>** carMap);

	glm::vec3 calcMovement(Road*** roads, vector<Direction>** carMap, double deltaT);

	void trafficJunctionControl(glm::vec3 moveVec, glm::vec3 targetPos, Road* currentRoad, Road* nextRoad, std::pair <int, int> nxt, vector<Direction>** carMap, Direction entryDir, double deltaT, Road*** roads);

	void moveCar(glm::vec3 moveVec, vector<Direction>** carMap, Road*** roads);

};
