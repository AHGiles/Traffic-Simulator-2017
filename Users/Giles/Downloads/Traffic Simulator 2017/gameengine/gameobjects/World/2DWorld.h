#pragma once

#include <Windows.h>
#include <Commdlg.h>
#include <iostream>
#include <fstream>

//Helpers/Librarys
#include "../ModelLibrary.h"
#include "../../OpenGLHelper.h"

//Objects
#include "../GUI/GUI.h"
#include "../Roads/Road.h"
#include "../Roads/Straight.h"
#include "../Roads/Empty.h"
#include "../Roads/Corner.h"
#include "../Roads/TJunc.h"
#include "../Roads/Crossroad.h"
#include "../../MapGen/RoadMapGen.h"


#define INITGRIDSIZE 25



class World2D {
//Variables and methods that 3D world needs to know about
public:
	Road*** roadGrid;
	int* roadMap;
	int gridSize;
	int tileSize;
	std::vector<glm::vec2> invalidCells;

	ModelLibrary* models;
	

	//Drawing
	Shader* roadShader;
	Shader* guiShader;

//Variables and methods that will be called by external unknown classes
public:
	World2D() {}

	World2D(ModelLibrary* models) { this->models = models; }

	void Init();

	void Update(double deltaT);

	void Draw(glm::vec4 normalisedCoord, Camera cam);

	void genMap();

	std::vector<glm::vec2> validateMap();

	void Raytrace(int HEIGHT, int WIDTH, int &mouseX, int &mouseY, Camera* camera);

	GUI* gui;

//Variables and methods that are just used in 2Dworld


	void loadFile();

	void saveFile();

	void reSize(int newSize);

	void deleteGrids();

	void addRoadPiece(glm::vec2 cellPos, int orientation);
};