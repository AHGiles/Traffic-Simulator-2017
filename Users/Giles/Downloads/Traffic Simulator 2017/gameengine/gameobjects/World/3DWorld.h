#pragma once

#include "2DWorld.h"
#include "../Car/Car.h"


class World3D : public World2D {

private:

	vector<Car*> cars;

	Shader* cubeMapShader;
	Shader* carShader;
	Shader* heatMapShader;

	bool heatMapOn;

	AssimpModel carModel;

	void swapModels();
	AssimpModel** emptyWeighting();

public:

	World3D() {}

	World3D(ModelLibrary* models);

	void Init(Road*** roadGrid, int* roadMap, int gridSize, int tileSize, GUI* gui, Shader* roadShader);

	void Update(double deltaT);

	void Draw(glm::vec4 normalisedCoord, Camera cam);

	void endSim();

	void Raytrace(int HEIGHT, int WIDTH, int &mouseX, int &mouseY, Camera* camera);

	void prevActiveCar();

	void nextActiveCar();

	vector<Direction>** carMap;

	int emptyType = 0;
	float simTime;
	bool carCam;
	int activeCar;
};
