#include "3DWorld.h"

World3D::World3D(ModelLibrary* models){
	this->models = models;


	this->heatMapOn = false;
	this->carCam = false;
	this->activeCar = 0;
	//Shaders
	this->carShader = new Shader("Shader/Shaders/carShader");
	this->heatMapShader = new Shader("Shader/Shaders/heatmapShader");

}

void World3D::Init(Road*** roadGrid, int* roadMap, int gridSize, int tileSize, GUI* gui, Shader* roadShader){


	//Grab the details from 2DWorld
	this->roadGrid = roadGrid;
	this->roadMap = roadMap;
	this->gridSize = gridSize;
	this->tileSize = tileSize;
	this->gui = gui;
	this->roadShader = roadShader;
	this->emptyType = this->gui->checkEmptyWeight();

	//allocate space for car map
	carMap = new vector<Direction>*[gridSize];
	for (int i = 0; i < gridSize; i++)
		carMap[i] = new vector<Direction>[gridSize];

	//Start the sim timer at 0
	this->simTime = 0;

	//Models
	swapModels();

	//get a list of straights
	vector<glm::vec2> strList = vector<glm::vec2>();
	//set all car map values to 0
	for (int i = 0; i < gridSize; i++) {
		for (int j = 0; j < gridSize; j++) {
			carMap[i][j] = vector<Direction>();
			if (roadGrid[i][j]->roadType == STRAIGHT)
				strList.push_back(roadGrid[i][j]->cellPos);
			if (roadGrid[i][j]->roadType == EMPTY)
				roadGrid[i][j]->setModel(emptyWeighting());
		}
	}

	//Get the chosen density of cars
	float numCarsMod = 0.25;
	for each (pair<Button*,float> b in gui->numCarsButtons)
	{
		if (b.first->isSelected)
			numCarsMod = b.second;
	}

	//add the number of cars to straights in the road map
	int numCars = strList.size()*numCarsMod;
	while(strList.size() > 0 && numCars > 0){
		int i = rand() % strList.size();
		int x = strList.at(i).x;
		int y = strList.at(i).y;
		glm::vec3 worldPos;
		//foundStraight = true;
		Road* temp = roadGrid[x][y];
		temp->timesTraversed++;
		Direction dir;
		switch (temp->roadLinks.at(0)->dir) {
			case NORTH:
				dir = Direction::NORTH;
				carMap[x][y].push_back(NORTH);
				worldPos = roadGrid[x][y]->roadLinks[0]->worldPosIn + glm::vec3(0, 0, 25);
				break;
			case EAST:
				dir = Direction::EAST;
				carMap[x][y].push_back(EAST);
				worldPos = roadGrid[x][y]->roadLinks[0]->worldPosIn + glm::vec3(25, 0, 0);
				break;
			case SOUTH:
				dir = Direction::SOUTH;
				carMap[x][y].push_back(SOUTH);
				worldPos = roadGrid[x][y]->roadLinks[1]->worldPosIn + glm::vec3(0, 0, -25);
				break;
			case WEST:
				dir = Direction::WEST;
				carMap[x][y].push_back(WEST);
				worldPos = roadGrid[x][y]->roadLinks[0]->worldPosIn + glm::vec3(-25, 0, 0);
				break;
		}
		Car* car = new Car(temp->cellPos, worldPos, dir, models->getACar(), &this->gridSize, &this->tileSize);
		//car->Init(this->roadMap);
		this->cars.push_back(car);
		numCars--;
		strList.erase(strList.begin() + i);
	}
	for each (Car* c in cars)
	{
		c->Init(this->roadMap, carMap);
	}

	//spawn car(s) on the road by adding to the cars list.  must be on a valid starting cell (straight or corner road) with a valid direction,
	//direction will be where they would have come from to get to that cell
}

void World3D::Update(double deltaT){

	//Update all the roads in the world
	for (int y = 0; y < this->gridSize; y++) {
		for (int x = 0; x < this->gridSize; x++) {
			this->roadGrid[x][y]->Update(deltaT);
			carMap[x][y] = std::vector<Direction>();
		}
	}


	this->simTime += deltaT;


	//Update road stats
	for (Car* car : cars) {
		carMap[(int)car->cellPos.x][(int)car->cellPos.y].push_back(car->direction);

		this->roadGrid[(int)car->cellPos.x][(int)car->cellPos.y]->timeOccupiedByCar += deltaT;
		if (!this->roadGrid[(int)car->cellPos.x][(int)car->cellPos.y]->activeTraffic.empty()) {
			switch (this->roadGrid[(int)car->cellPos.x][(int)car->cellPos.y]->activeTraffic.front()) {
			case NORTH:
				this->roadGrid[(int)car->cellPos.x][(int)car->cellPos.y]->lights[0]++;
				break;
			case EAST:
				this->roadGrid[(int)car->cellPos.x][(int)car->cellPos.y]->lights[1]++;
				break;
			case SOUTH:
				this->roadGrid[(int)car->cellPos.x][(int)car->cellPos.y]->lights[2]++;
				break;
			case WEST:
				this->roadGrid[(int)car->cellPos.x][(int)car->cellPos.y]->lights[3]++;
				break;
			}
		}
	}
	
	//update all the cars in the world
	for (Car* car : cars) {
		car->Update(roadMap, roadGrid, carMap, deltaT);
	}

	//Check for heatmap activation
	this->heatMapOn = gui->checkHeatmapStatus();
}

void World3D::Draw(glm::vec4 normalisedCoord, Camera cam){

	if (carCam) {
		if (cars.size() > 0)
			cam.changeCam(cars[activeCar]->worldPos - (cars[activeCar]->moveVec * 20.0f) + glm::vec3(0,10,0), glm::normalize((cars[activeCar]->moveVec * 20.0f) - glm::vec3(0, 10, 0)), glm::vec3(0, 1, 0));
		else
			carCam = false;
	}

	//draw all the cars in the world
	for (Car* car : cars) {
		car->Draw(this->carShader, cam);
	}
	
	//glEnable(GL_BLEND);
	glDisable(GL_CULL_FACE);
	//For loop over roadGrid drawing each element
	for (int i = 0; i < this->gridSize; i++) {
		for (int j = 0; j < this->gridSize; j++) {
			if(!this->heatMapOn)
				this->roadGrid[i][j]->Draw(this->roadShader, cam);
			else
				this->roadGrid[i][j]->DrawHeatMap(this->heatMapShader, cam, this->simTime);
		}
	}
	glEnable(GL_CULL_FACE);
	//glDisable(GL_BLEND);

	this->gui->Draw(normalisedCoord);
}

void World3D::endSim() {

	//clear traffic lights
	for (int i = 0; i < this->gridSize; i++) {
		for (int j = 0; j < this->gridSize; j++) {
			while (!this->roadGrid[i][j]->activeTraffic.empty()) {
				this->roadGrid[i][j]->activeTraffic.pop();
			}
			this->roadGrid[i][j]->timeOccupiedByCar = 0;
		}
	}

	//Swap models
	swapModels();

	//Delete car lists
	this->cars.clear();
}

void World3D::Raytrace(int HEIGHT, int WIDTH, int & mouseX, int & mouseY, Camera * camera){
	for (int y = 0; y < gridSize; y++)
	{
		for (int x = 0; x < gridSize; x++)
		{
			if (roadGrid[x][y]->isSelected != 3)
				roadGrid[x][y]->isSelected = 0;
		}
	}

	Ray ray = Ray(mouseX, HEIGHT - mouseY, WIDTH, HEIGHT, camera);
	glm::vec3 rayDir = ray.getRayDir();

	glm::vec3 camPos = camera->getPos();
	float modifier = camPos.y / -rayDir.y;

	glm::vec3 worldCoords = glm::vec3(camPos.x + (rayDir.x * modifier), 0, camPos.z + (rayDir.z * modifier));

	glm::vec2 selectedCell = glm::vec2((int)worldCoords.x / tileSize, (int)worldCoords.z / tileSize);
	if (selectedCell.x >= 0 && selectedCell.x < gridSize && selectedCell.y >= 0 && selectedCell.y < gridSize)
	{
		roadGrid[(int)selectedCell.x][(int)selectedCell.y]->isSelected = 1;
	}

}

void World3D::prevActiveCar()
{
	if (activeCar == 0) {
		activeCar = cars.size() - 1;
	}
	else
		activeCar--;
	cout << "Current car = " << activeCar << endl;
}

void World3D::nextActiveCar()
{
	if (activeCar == cars.size() - 1) {
		activeCar = 0;
	}
	else
		activeCar++;
	cout << "Current car = " << activeCar << endl;
}

void World3D::swapModels(){

	//Swap Models
	models->swapModels();

}

AssimpModel** World3D::emptyWeighting(){
	int testVal = rand() % 100;

	if (emptyType == 0) // If average selection
	{
		if (testVal < 30)
			return models->getEmptyAt(8);
		if (testVal < 35)
			return models->getEmptyAt(0);
		if (testVal < 37)
			return models->getEmptyAt(1);
		if (testVal < 60)
			return models->getEmptyAt(7);
		if (testVal < 75)
			return models->getEmptyAt(3);
		if (testVal < 90)
			return models->getEmptyAt(4);
		if (testVal < 95)
			return models->getEmptyAt(5);
		if (testVal < 100)
			return models->getEmptyAt(6);
	}
	if (emptyType == 1) // If tree selection
	{

		if (testVal < 30)
			return models->getEmptyAt(8);
		if (testVal < 40)
			return models->getEmptyAt(0);
		if (testVal < 41)
			return models->getEmptyAt(1);
		if (testVal < 90)
			return models->getEmptyAt(7);
		if (testVal < 91)
			return models->getEmptyAt(3);
		if (testVal < 92)
			return models->getEmptyAt(4);
		if (testVal < 96)
			return models->getEmptyAt(5);
		if (testVal < 100)
			return models->getEmptyAt(6);
	}
	if (emptyType == 2) // If housing selection
	{

		if (testVal < 10)
			return models->getEmptyAt(8);
		if (testVal < 12)
			return models->getEmptyAt(0);
		if (testVal < 16)
			return models->getEmptyAt(1);
		if (testVal < 20)
			return models->getEmptyAt(7);
		if (testVal < 45)
			return models->getEmptyAt(3);
		if (testVal < 80)
			return models->getEmptyAt(4);
		if (testVal < 90)
			return models->getEmptyAt(5);
		if (testVal < 100)
			return models->getEmptyAt(6);
	}
}
