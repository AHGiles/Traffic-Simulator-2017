#include "2DWorld.h"

void World2D::Init() {

	//Shaders
	this->roadShader = new Shader("Shader/Shaders/roadShader");
	this->guiShader = new Shader("Shader/Shaders/GUIShader");

	//Models
	//this->models->setEmpty("assets/models/Empty.obj");

	//this->models->setStraight("assets/models/Straight.obj");

	//this->models->setCorner("assets/models/Corner.obj");

	//this->models->setTJunc("assets/models/TJunc.obj");

	//this->models->setCross("assets/models/Crossroads.obj");

	//GUI
	this->gui = new GUI(this->guiShader);
	this->gui->Init();


	//RoadList
	this->gridSize = INITGRIDSIZE;
	this->tileSize = TILESIZE;
	//create array of Roads
	this->roadGrid = new Road**[this->gridSize];
	for (int i = 0; i < this->gridSize; ++i) {
		this->roadGrid[i] = new Road*[this->gridSize];
	}

	this->roadMap = new int[INITGRIDSIZE * INITGRIDSIZE];

	//populate array with empty road type
	for (int i = 0; i < this->gridSize; i++) {
		for (int j = 0; j < this->gridSize; j++) {
			this->roadGrid[i][j] = new Empty(glm::vec2(i, j), glm::vec3((i * TILESIZE) + TILESIZE / 2, 0, (j * TILESIZE) + TILESIZE / 2), models->getAnEmpty());
			this->roadMap[i * INITGRIDSIZE + j] = 0;
		}
	}

	invalidCells = std::vector<glm::vec2>();
}

void World2D::Update(double deltaT) {

	//Raytrace(HEIGHT, WIDTH, mouseX, mouseY,  camera);

	//Update all the roads in the world
	//for (int y = 0; y < this->gridSize; y++) {
	//	for (int x = 0; x < this->gridSize; x++) {
	//		this->roadGrid[x][y]->Update(deltaT);
	//	}
	//}
	
	for each (glm::vec2 p in invalidCells)
	{
		roadGrid[(int)p.x][(int)p.y]->isSelected = 3;
	}

	//method to check to see if new size has been set in gui
	int newSize = this->gui->checkResizeButtons();
	if (newSize != -1 && newSize != this->gridSize)
		reSize(newSize);

	//method to check if a new random map needs to be made
	if (this->gui->checkGenMap())
		genMap();

	//check if load or save has been pressed
	switch (gui->checkSaveLoad()) {
		case 1:
			this->saveFile();
			break;
		case 2:
			this->loadFile();
			break;
	}

}

void World2D::Draw(glm::vec4 normalisedCoord, Camera cam) {

	

	//For loop over roadGrid drawing each element
	//this->roadShader->Bind();
	for (int i = 0; i < this->gridSize; i++) {
		for (int j = 0; j < this->gridSize; j++) {
			//if (roadGrid[i][j]->roadType != EMPTY) {
				this->roadGrid[i][j]->Draw(this->roadShader, cam);
			//}
		}
	}
	this->gui->Draw(normalisedCoord);
	//draw all the cars in the world

}

void World2D::deleteGrids(){
	delete[] this->roadMap; //delete int*

	for (int i = 0; i < this->gridSize; ++i) { //delete Road**
		delete[] this->roadGrid[i];
	}
	delete[] this->roadGrid; //delete Road*
}

void World2D::reSize(int newSize) {

	//Delete old stufffs
	deleteGrids();

	this->gridSize = newSize;

	//create array of Roads
	this->roadGrid = new Road**[this->gridSize];
	for (int i = 0; i < this->gridSize; ++i) {
		this->roadGrid[i] = new Road*[this->gridSize];
	}

	this->roadMap = new int[this->gridSize * this->gridSize];

	//populate array with empty road type
	for (int i = 0; i < this->gridSize; i++) {
		for (int j = 0; j < this->gridSize; j++) {
			this->roadGrid[i][j] = new Empty(glm::vec2(i, j), glm::vec3((i * TILESIZE) + TILESIZE / 2, 0, (j * TILESIZE) + TILESIZE / 2), models->getAnEmpty());
			this->roadMap[i * this->gridSize + j] = 0;
		}
	}

}

void World2D::addRoadPiece(glm::vec2 cellPos, int orientation) {
	int x = cellPos.x;
	int y = cellPos.y;

	delete this->roadGrid[x][y];

	switch (orientation)
	{
	case 0:
		this->roadGrid[x][y] = new Empty(cellPos, glm::vec3((x * TILESIZE) + TILESIZE / 2, 0, (y * TILESIZE) + TILESIZE / 2), models->getAnEmpty());
		break;
	case 1:
		/*NS*/this->roadGrid[x][y] = new Straight(cellPos, glm::vec3((x * TILESIZE) + TILESIZE / 2, 0, (y * TILESIZE) + TILESIZE / 2), models->getStraight());
		/*WE*/this->roadGrid[x][y]->rotate();
		break;
	case 2:
		/*NS*/this->roadGrid[x][y] = new Straight(cellPos, glm::vec3((x * TILESIZE) + TILESIZE / 2, 0, (y * TILESIZE) + TILESIZE / 2), models->getStraight());
		break;
	case 3://SE
		/*NE*/this->roadGrid[x][y] = new Corner(cellPos, glm::vec3((x * TILESIZE) + TILESIZE / 2, 0, (y * TILESIZE) + TILESIZE / 2), models->getCorner());
		/*ES*/this->roadGrid[x][y]->rotate();
		break;
	case 4://SW
		/*NE*/this->roadGrid[x][y] = new Corner(cellPos, glm::vec3((x * TILESIZE) + TILESIZE / 2, 0, (y * TILESIZE) + TILESIZE / 2), models->getCorner());
		/*ES*/this->roadGrid[x][y]->rotate();
		/*SW*/this->roadGrid[x][y]->rotate();
		break;
	case 5://NW
		/*NE*/this->roadGrid[x][y] = new Corner(cellPos, glm::vec3((x * TILESIZE) + TILESIZE / 2, 0, (y * TILESIZE) + TILESIZE / 2), models->getCorner());
		/*ES*/this->roadGrid[x][y]->rotate();
		/*SW*/this->roadGrid[x][y]->rotate();
		/*WN*/this->roadGrid[x][y]->rotate();
		break;
	case 6://NE
		/*NE*/this->roadGrid[x][y] = new Corner(cellPos, glm::vec3((x * TILESIZE) + TILESIZE / 2, 0, (y * TILESIZE) + TILESIZE / 2), models->getCorner());
		break;
	case 7:
		/*ALL*/this->roadGrid[x][y] = new Crossroad(cellPos, glm::vec3((x * TILESIZE) + TILESIZE / 2, 0, (y * TILESIZE) + TILESIZE / 2), models->getCross());
		break;
	case 8://SEW
		/*NSE*/this->roadGrid[x][y] = new TJunc(cellPos, glm::vec3((x * TILESIZE) + TILESIZE / 2, 0, (y * TILESIZE) + TILESIZE / 2), models->getTJunc());
		/*WES*/this->roadGrid[x][y]->rotate();
		break;
	case 9://WSN
		/*NSE*/this->roadGrid[x][y] = new TJunc(cellPos, glm::vec3((x * TILESIZE) + TILESIZE / 2, 0, (y * TILESIZE) + TILESIZE / 2), models->getTJunc());
		/*WES*/this->roadGrid[x][y]->rotate();
		/*SNW*/this->roadGrid[x][y]->rotate();
		break;
	case 10://NEW
		/*NSE*/this->roadGrid[x][y] = new TJunc(cellPos, glm::vec3((x * TILESIZE) + TILESIZE / 2, 0, (y * TILESIZE) + TILESIZE / 2), models->getTJunc());
		/*WES*/this->roadGrid[x][y]->rotate();
		/*SNW*/this->roadGrid[x][y]->rotate();
		/*NEW*/this->roadGrid[x][y]->rotate();
		break;
	case 11://NSE
		/*NSE*/this->roadGrid[x][y] = new TJunc(cellPos, glm::vec3((x * TILESIZE) + TILESIZE / 2, 0, (y * TILESIZE) + TILESIZE / 2), models->getTJunc());
		break;
	default:
		this->roadGrid[x][y] = new Empty(cellPos, glm::vec3((x * TILESIZE) + TILESIZE / 2, 0, (y * TILESIZE) + TILESIZE / 2), models->getTJunc());
		cout << "cry" << endl;
		break;
	}
	this->roadMap[y * gridSize + x] = orientation;

	for (int i = 0; i < invalidCells.size(); i++) {
		if (invalidCells[i].x == x && invalidCells[i].y == y) {
			invalidCells.erase(invalidCells.begin() + i);
			break;
		}
	}
}

void World2D::Raytrace(int HEIGHT, int WIDTH, int &mouseX, int &mouseY, Camera* camera)
{

	for (int y = 0; y < gridSize; y++) 
	{
		for (int x = 0; x < gridSize; x++) 
		{
			if(roadGrid[x][y]->isSelected != 3)
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

void World2D::genMap() {

	RoadMapGen generator(this->gridSize, this->gridSize);
	vector<vector<int>> roadMap = generator.genRoadMap();
	for (int y = 0; y < this->gridSize; y++) {
		for (int x = 0; x < this->gridSize; x++) {
			addRoadPiece(glm::vec2(x, y), roadMap[x][y]);
		}
	}


}

std::vector<glm::vec2> World2D::validateMap()
{
	for (int y = 0; y < gridSize; y++)
	{
		for (int x = 0; x < gridSize; x++)
		{
			roadGrid[x][y]->isSelected = 0;
		}
	}
	std::vector<glm::vec2> badCells = std::vector<glm::vec2>();

	for (int x = 0; x < gridSize; x++){
		for (int y = 0; y < gridSize; y++) {
			switch (roadMap[y * gridSize + x]) {
			case 1://STR_WE,		//1		--
				if (x - 1 >= 0 && x + 1 < gridSize) {
					int l, r;
					l = roadMap[y * gridSize + (x - 1)];
					r = roadMap[y * gridSize + (x + 1)];
					if (l == 0 || r == 0 || l == 2 || l == 3 || l == 6 || l == 11 || r == 2|| r == 4 || r == 5 || r == 9)
						badCells.push_back(glm::vec2(x, y));
				}
				else{
					badCells.push_back(glm::vec2(x, y));
				}
				break;
			case 2://STR_NS,		//2		|
				if (y - 1 >= 0 && y + 1 < gridSize) {
					int u, d;
					u = roadMap[(y - 1) * gridSize + x];
					d = roadMap[(y + 1) * gridSize + x];
					if (u == 0 || d == 0 || u == 1 || u == 3 || u == 4 || u == 8 || d == 1 || d == 5|| d == 6 || d == 10)
						badCells.push_back(glm::vec2(x, y));
				}
				else {
					badCells.push_back(glm::vec2(x, y));
				}
				break;
			case 3://COR_NW,		//3		_|
				if (x - 1 >= 0 && y - 1 >= 0) {
					int u, l;
					l = roadMap[y * gridSize + (x - 1)];
					u = roadMap[(y - 1) * gridSize + x];
					if (l == 0 || u == 0 || l == 2 || l == 3 || l == 6 || l == 11 || u == 1 || u == 3 || u == 4 || u == 8)
						badCells.push_back(glm::vec2(x, y));
				}
				else {
					badCells.push_back(glm::vec2(x, y));
				}
				break;
			case 4://COR_NE,		//4		|_
				if (y - 1 >= 0 && x + 1 < gridSize) {
					int u, r;
					r = roadMap[y * gridSize + (x + 1)];
					u = roadMap[(y - 1) * gridSize + x];
					if (r == 0 || u == 0 || r == 2 || r == 4 || r == 5 || r == 9 || u == 1 || u == 3 || u == 4 || u == 8)
						badCells.push_back(glm::vec2(x, y));
				}
				else {
					badCells.push_back(glm::vec2(x, y));
				}
				break;
			case 5://COR_SE,		//5		|"
				if (y + 1 < gridSize && x + 1 < gridSize) {
					int d,r;
					r = roadMap[y * gridSize + (x + 1)];
					d = roadMap[(y + 1) * gridSize + x];
					if (r == 0 || d == 0 || r == 2 || r == 4 || r == 5 || r == 9 || d == 1 || d == 5 || d == 6 || d == 10)
						badCells.push_back(glm::vec2(x, y));
				}
				else {
					badCells.push_back(glm::vec2(x, y));
				}
				break;
			case 6://COR_SW,		//6		"|
				if (x - 1 >= 0 && y + 1 < gridSize) {
					int d, l;
					l = roadMap[y * gridSize + (x - 1)];
					d = roadMap[(y + 1) * gridSize + x];
					if (l == 0 || d == 0 || l == 2 || l == 3 || l == 6 || l == 11 || d == 1 || d == 5 || d == 6 || d == 10)
						badCells.push_back(glm::vec2(x, y));
				}
				else {
					badCells.push_back(glm::vec2(x, y));
				}
				break;
			case 7://CRS_RD,		//7		+
				if (x - 1 >= 0 && y + 1 < gridSize && y - 1 >= 0 && x + 1 < gridSize) {
					int u, d, l, r;
					l = roadMap[y * gridSize + (x - 1)];
					r = roadMap[y * gridSize + (x + 1)];
					u = roadMap[(y - 1) * gridSize + x];
					d = roadMap[(y + 1) * gridSize + x];
					if (l == 0 || r == 0 || u == 0 || d == 0 || l == 2 || l == 3 || l == 6 || l == 11 || r == 2 || r == 4 || r == 5 || r == 9 || u == 1 || u == 3 || u == 4 || u == 8 || d == 1 || d == 5 || d == 6 || d == 10
						|| l > 6 || d > 6 || r > 6 || u > 6)
						badCells.push_back(glm::vec2(x, y));
				}
				else {
					badCells.push_back(glm::vec2(x, y));
				}
				break;
			case 8://TJNC_NWE,	//8		_|_
				if (x - 1 >= 0 && y - 1 >= 0 && x + 1 < gridSize) {
					int u, l, r;
					l = roadMap[y * gridSize + (x - 1)];
					r = roadMap[y * gridSize + (x + 1)];
					u = roadMap[(y - 1) * gridSize + x];
					if (l == 0 || r == 0 || u == 0 || l == 2 || l == 3 || l == 6 || l == 11 || r == 2 || r == 4 || r == 5 || r == 9 || u == 1 || u == 3 || u == 4 || u == 8
						|| l > 6  || r > 6 || u > 6)
						badCells.push_back(glm::vec2(x, y));
				}
				else {
					badCells.push_back(glm::vec2(x, y));
				}
				break;
			case 9://TJNC_ENS,	//9		|-
				if ( y + 1 < gridSize && y - 1 >= 0 && x + 1 < gridSize) {
					int u, d, r;
					r = roadMap[y * gridSize + (x + 1)];
					u = roadMap[(y - 1) * gridSize + x];
					d = roadMap[(y + 1) * gridSize + x];
					if (r == 0 || u == 0 || d == 0 || r == 2 || r == 4 || r == 5 || r == 9 || u == 1 || u == 3 || u == 4 || u == 8 || d == 1 || d == 5 || d == 6 || d == 10
						|| d > 6 || r > 6 || u > 6)
						badCells.push_back(glm::vec2(x, y));
				}
				else {
					badCells.push_back(glm::vec2(x, y));
				}
				break;
			case 10://TJNC_SEW,	//10	"|"
				if (x - 1 >= 0 && y + 1 < gridSize && x + 1 < gridSize) {
					int d, l, r;
					l = roadMap[y * gridSize + (x - 1)];
					r = roadMap[y * gridSize + (x + 1)];
					d = roadMap[(y + 1) * gridSize + x];
					if (l == 0 || r == 0 || d == 0 || l == 2 || l == 3 || l == 6 || l == 11 || r == 2 || r == 4 || r == 5 || r == 9 || d == 1 || d == 5 || d == 6 || d == 10
						|| l > 6 || d > 6 || r > 6)
						badCells.push_back(glm::vec2(x, y));
				}
				else {
					badCells.push_back(glm::vec2(x, y));
				}
				break;
			case 11://TJNC_WSN	//11	-|
				if (x - 1 >= 0 && y + 1 < gridSize && y - 1 >= 0) {
					int u, d, l;
					l = roadMap[y * gridSize + (x - 1)];
					u = roadMap[(y - 1) * gridSize + x];
					d = roadMap[(y + 1) * gridSize + x];
					if (l == 0 || u == 0 || d == 0 || l == 2 || l == 3 || l == 6 || l == 11 || u == 1 || u == 3 || u == 4 || u == 8 || d == 1 || d == 5 || d == 6 || d == 10
						|| l > 6 || d > 6 || u > 6)
						badCells.push_back(glm::vec2(x, y));
				}
				else {
					badCells.push_back(glm::vec2(x, y));
				}
				break;
			}
		}
	}
	if (badCells.size() > 0) {
		MessageBox(NULL,"The map created is invalid","Invalid Map",MB_ICONWARNING | MB_OK);
	}
		

	return badCells;
}

void World2D::loadFile() {
	OPENFILENAME ofn;
	// a another memory buffer to contain the file name
	char szFile[1000];
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "ROADMAP\0*.ROADMAP\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetOpenFileName(&ofn)) {
		/********************************
		ADDED THIS BIT
		*********************************/
		// load file at given filename
		ifstream myfile;
		myfile.open(ofn.lpstrFile);
		int size;
		myfile >> size;
		reSize(size);
		for (int y = 0; y < gridSize; y++) {
			for (int x = 0; x < gridSize; x++) {
				int next;
				myfile >> next;
				addRoadPiece(glm::vec2(x, y), next);
			}
		}
		gui->setGridSizeSelected(size);
		myfile.close();
	}
	else
	{
		// All this stuff below is to tell you exactly how you messed up above. 
		// Once you've got that fixed, you can often (not always!) reduce it to a 'user cancelled' assumption.
		switch (CommDlgExtendedError())
		{
		case CDERR_DIALOGFAILURE: std::cout << "CDERR_DIALOGFAILURE\n";   break;
		case CDERR_FINDRESFAILURE: std::cout << "CDERR_FINDRESFAILURE\n";  break;
		case CDERR_INITIALIZATION: std::cout << "CDERR_INITIALIZATION\n";  break;
		case CDERR_LOADRESFAILURE: std::cout << "CDERR_LOADRESFAILURE\n";  break;
		case CDERR_LOADSTRFAILURE: std::cout << "CDERR_LOADSTRFAILURE\n";  break;
		case CDERR_LOCKRESFAILURE: std::cout << "CDERR_LOCKRESFAILURE\n";  break;
		case CDERR_MEMALLOCFAILURE: std::cout << "CDERR_MEMALLOCFAILURE\n"; break;
		case CDERR_MEMLOCKFAILURE: std::cout << "CDERR_MEMLOCKFAILURE\n";  break;
		case CDERR_NOHINSTANCE: std::cout << "CDERR_NOHINSTANCE\n";     break;
		case CDERR_NOHOOK: std::cout << "CDERR_NOHOOK\n";          break;
		case CDERR_NOTEMPLATE: std::cout << "CDERR_NOTEMPLATE\n";      break;
		case CDERR_STRUCTSIZE: std::cout << "CDERR_STRUCTSIZE\n";      break;
		case FNERR_BUFFERTOOSMALL: std::cout << "FNERR_BUFFERTOOSMALL\n";  break;
		case FNERR_INVALIDFILENAME: std::cout << "FNERR_INVALIDFILENAME\n"; break;
		case FNERR_SUBCLASSFAILURE: std::cout << "FNERR_SUBCLASSFAILURE\n"; break;
		default: std::cout << "You cancelled.\n";
		}
	}

	

}

void World2D::saveFile() {
	OPENFILENAME ofn;
	// a another memory buffer to contain the file name
	char szFile[1000];
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "ROADMAP\0*.ROADMAP\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetSaveFileName(&ofn)) {
		/********************************
		ADDED THIS BIT
		*********************************/
		// save file at given filename
		ofstream myfile;
		string fileName = (string)ofn.lpstrFile + ".ROADMAP";
		myfile.open(fileName);
		myfile << gridSize << "\n";
		for (int y = 0; y < gridSize; y++) {
			for (int x = 0; x < gridSize; x++) {
				myfile << roadMap[y * gridSize + x] << " ";
			}
			myfile << "\n";
		}
		myfile.close();
	}
	else
	{
		// All this stuff below is to tell you exactly how you messed up above. 
		// Once you've got that fixed, you can often (not always!) reduce it to a 'user cancelled' assumption.
		switch (CommDlgExtendedError())
		{
		case CDERR_DIALOGFAILURE: std::cout << "CDERR_DIALOGFAILURE\n";   break;
		case CDERR_FINDRESFAILURE: std::cout << "CDERR_FINDRESFAILURE\n";  break;
		case CDERR_INITIALIZATION: std::cout << "CDERR_INITIALIZATION\n";  break;
		case CDERR_LOADRESFAILURE: std::cout << "CDERR_LOADRESFAILURE\n";  break;
		case CDERR_LOADSTRFAILURE: std::cout << "CDERR_LOADSTRFAILURE\n";  break;
		case CDERR_LOCKRESFAILURE: std::cout << "CDERR_LOCKRESFAILURE\n";  break;
		case CDERR_MEMALLOCFAILURE: std::cout << "CDERR_MEMALLOCFAILURE\n"; break;
		case CDERR_MEMLOCKFAILURE: std::cout << "CDERR_MEMLOCKFAILURE\n";  break;
		case CDERR_NOHINSTANCE: std::cout << "CDERR_NOHINSTANCE\n";     break;
		case CDERR_NOHOOK: std::cout << "CDERR_NOHOOK\n";          break;
		case CDERR_NOTEMPLATE: std::cout << "CDERR_NOTEMPLATE\n";      break;
		case CDERR_STRUCTSIZE: std::cout << "CDERR_STRUCTSIZE\n";      break;
		case FNERR_BUFFERTOOSMALL: std::cout << "FNERR_BUFFERTOOSMALL\n";  break;
		case FNERR_INVALIDFILENAME: std::cout << "FNERR_INVALIDFILENAME\n"; break;
		case FNERR_SUBCLASSFAILURE: std::cout << "FNERR_SUBCLASSFAILURE\n"; break;
		default: std::cout << "You cancelled.\n";
		}
	}

}
