#pragma once

#include "RoadDetails.h"
#include <vector>
#include <queue>
#include "../GameObjects/Transform.h"
#include "../Shader/Shader.h"
#include "../GameObjects/Camera.h"
#include "../OpenGLHelper.h"
#include "../../ObjectLoading/Assimp/AssimpModel.h"


enum RoadType { EMPTY = 1, STRAIGHT = 2, CORNER = 3, TJUNC = 4, CROSSROADS = 5 };

class Road {
public:
	Road() {}

	virtual void rotate() {};

	void Draw(Shader* roadShader, Camera cam);
	void DrawHeatMap(Shader* heatmapShader, Camera cam, int simTime);
	virtual void Update(double deltaT) {};

	void setModel(AssimpModel** newModel);

	string toString(int simTime);
	string getRoadType();
	string getMostActiveLight();

	RoadType roadType;
	glm::vec2 cellPos;
	glm::vec3 worldPos;

	//vector that contains details on all entrances and exits from this road tile
	std::vector<RoadDetails*> roadLinks;

	//queue structure to store traffic data for traffic info
	std::queue<Direction> activeTraffic;
	int lights[4];

	float timeOccupiedByCar;
	int timesTraversed;
	int isSelected;
protected:
	AssimpModel** model;
	Transform transform;
	

	
};