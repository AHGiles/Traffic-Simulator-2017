#pragma once
#include "../GameObjects/Primitives/Mesh.h"
#include "../ObjectLoading/Texture.h"
#include <vector>
#include "../IO/KeyboardInput.h"
#include "../gameobjects/Primitives/Ray.h"
#include "Button.h"

#define GUIBORDER_X 0.05
#define GUIBORDER_Y 0.04

#define MULTIBUTTONSIZE_X 0.1
#define MULTIBUTTONSIZE_Y 0.1

#define SINGLEBUTTONSIZE_X 0.2
#define SINGLEBUTTONSIZE_Y 0.07

#define ROADBUTTONSIZE_X 0.14
#define ROADBUTTONSIZE_Y 0.17
#define ROADBUTTONGAP_X 0.05
#define ROADBUTTONGAP_Y 0.025



class GUI
{
public:
	GUI(Shader* guiShader);
	GUI(Mesh &m) { this->background = &m; };
	~GUI();

	void Init();
	void Draw(glm::vec4 normalisedCoords/*, KeyboardInput &k*/);
	void Update(KeyboardInput &k);

	void checkButtonCollision(glm::vec4 normalisedCoords);
	bool checkGUIcollision(glm::vec4 normalisedCoords);

	//Check Button Methods
	int checkResizeButtons();
	bool checkGenMap();
	int checkEmptyWeight();
	bool checkSimStart();
	bool checkSimEnd();
	int checkSaveLoad();
	bool checkHeatmapStatus();
	/********************************
	ADDED THIS BIT
	*********************************/
	void setGridSizeSelected(int size);

	bool simRunning;
	bool heatmapOn;

	int roadPieceSelected;
	Vert* verts;
	Shader* GUIShader;

	Mesh* background;
	Texture* textureGUI;

	//Build Sim Buttons
	std::vector < pair<Button*, int>> roadButtons;
	std::vector < pair<Button*, int>> gridSizeButtons;
	std::vector < pair<Button*, int>> loadSaveButtons;
	std::vector < pair<Button*, int>> emptyWeightButtons;
	std::vector < pair<Button*, float>> numCarsButtons;
	Button* genMap;
	Button* startSim;

	//Run Sim Buttons
	Button* endSim;
	Button* heatmap;

};