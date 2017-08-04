#include "GUI.h"

GUI::GUI(Shader* guiShader)
{ 
	this->verts = new Vert [4] {
		Vert(glm::vec3(-1.0,-1.0,0), glm::vec2(0.0,1.0), glm::vec3(0.0,0.0,-1.0)),
		Vert(glm::vec3(-0.5,-1.0,0), glm::vec2(1.0,1.0), glm::vec3(0.0,0.0,-1.0)),
		Vert(glm::vec3(-0.5,1.0,0), glm::vec2(1.0,0.0), glm::vec3(0.0,0.0,-1.0)),
		Vert(glm::vec3(-1.0,1.0,0), glm::vec2(0.0,0.0), glm::vec3(0.0,0.0,-1.0))

	};

	unsigned int indices[] = { 0,1,2,0,2,3 };

	this->background = new Mesh(verts, sizeof(verts), indices, sizeof(indices));
	roadPieceSelected = -1;
	this->GUIShader = guiShader;
}

GUI::~GUI()
{
}

void GUI::Init()
{
	this->simRunning = false;
	this->heatmapOn = false;

	//texture for GUI background
	this->textureGUI = new Texture("assets/textures/GUI_Background.png");

	/*****************************************GRID SIZE BUTTONS ********************************************/
	string gridTextureList[4] = { "assets/textures/button_10.png", "assets/textures/button_15.png", "assets/textures/button_20.png", "assets/textures/button_25.png" };

	float curMinY;

	//Create grid size buttons
	int gridSize[] = { 10,15,20,25 };
	for (size_t x = 0; x < 4; x++) {

		float	xMin = (-1 + GUIBORDER_X) + (x*MULTIBUTTONSIZE_X),
				yMin = (1 - GUIBORDER_Y) - MULTIBUTTONSIZE_Y,
				xMax = (-1 + GUIBORDER_X) + ((x + 1)*MULTIBUTTONSIZE_X),
				yMax = (1 - GUIBORDER_Y);

		curMinY = yMin;

		glm::vec3 buttonPos((xMin + xMax) / 2, (yMin + yMax) / 2, 0);
		glm::vec2 buttonDim(xMax - xMin, yMax - yMin);
		Texture* text = new Texture(gridTextureList[x]);
		Button* butt = new Button(buttonPos, buttonDim, text, this->GUIShader);
		if (gridSize[x] == 25)
			butt->isSelected = 2;
		this->gridSizeButtons.push_back(pair<Button*, int>(butt, gridSize[x]));
	}

	/*****************************************GEN MAP BUTTON ********************************************/

	float	xMin = (-1 + MULTIBUTTONSIZE_X + GUIBORDER_X),
			yMin = (curMinY - GUIBORDER_Y) - SINGLEBUTTONSIZE_Y,
			xMax = (-1 + MULTIBUTTONSIZE_X + GUIBORDER_X) + SINGLEBUTTONSIZE_X,
			yMax = (curMinY - GUIBORDER_Y);

	curMinY = yMin;

	glm::vec3 buttonPos((xMin + xMax) / 2, (yMin + yMax) / 2, 0);
	glm::vec2 buttonDim(xMax - xMin, yMax - yMin);

	Texture* text = new Texture("assets/textures/button_Gen.png");
	this->genMap = new Button(buttonPos, buttonDim, text, this->GUIShader);

	/*****************************************ROAD BUTTONS ********************************************/
	//Texture list for Road Buttons
	string roadTextureList[12] = {"assets/textures/button_GRASS.png", "assets/textures/button_EW.png", "assets/textures/button_NS.png",
		"assets/textures/button_SE.png", "assets/textures/button_SW.png", "assets/textures/button_NW.png",
		"assets/textures/button_NE.png", "assets/textures/button_CROSS.png", "assets/textures/button_EWN.png",
		"assets/textures/button_NSW.png", "assets/textures/button_EWS.png", "assets/textures/button_NSE.png"};



	//Create road buttons
	int x = 0;
	for (size_t i = 0; i < 6; i++)
	{
		//Button 1
		float	xMin = (-0.96 + GUIBORDER_X),
				yMin = (curMinY - ROADBUTTONSIZE_Y - ROADBUTTONGAP_Y),
				xMax = (-0.96 + GUIBORDER_X + ROADBUTTONSIZE_X),
				yMax = (curMinY - ROADBUTTONGAP_Y);

		glm::vec3 buttonPos((xMin + xMax) / 2, (yMin + yMax) / 2, 0);
		glm::vec2 buttonDim(xMax - xMin, yMax - yMin);
		Texture* text = new Texture(roadTextureList[i * 2]);
		Button* butt = new Button(buttonPos, buttonDim, text, this->GUIShader);
		this->roadButtons.push_back(pair<Button*, int>(butt, x));

		//Button 2
		xMin = (-0.96 + GUIBORDER_X + ROADBUTTONSIZE_X + ROADBUTTONGAP_X),
		xMax = (-0.96 + GUIBORDER_X + (2 * ROADBUTTONSIZE_X) + ROADBUTTONGAP_X);

		buttonPos = glm::vec3((xMin + xMax) / 2, (yMin + yMax) / 2, 0);
		buttonDim = glm::vec2(xMax - xMin, yMax - yMin);
		text = new Texture(roadTextureList[i * 2 + 1]);
		butt = new Button(buttonPos, buttonDim, text, this->GUIShader);
		this->roadButtons.push_back(pair<Button*, int>(butt, x+1));

		x+=2;
		curMinY = yMin;
	}

	/*****************************************LOAD SAVE BUTTONS ********************************************/

	//Button 1 (Save)
	xMin = (-1 + MULTIBUTTONSIZE_X + GUIBORDER_X),
	yMin = (curMinY - GUIBORDER_Y) - MULTIBUTTONSIZE_Y,
	xMax = (-1 + MULTIBUTTONSIZE_X + GUIBORDER_X) + MULTIBUTTONSIZE_X,
	yMax = (curMinY - GUIBORDER_Y);

	buttonPos = glm::vec3((xMin + xMax) / 2, (yMin + yMax) / 2, 0);
	buttonDim = glm::vec2(xMax - xMin, yMax - yMin);
	text = new Texture("assets/textures/button_Save.png");
	Button* butt = new Button(buttonPos, buttonDim, text, this->GUIShader);
	this->loadSaveButtons.push_back(pair<Button*, int>(butt, 1));

	//Button 2 (Load)
	xMin = (-1 + MULTIBUTTONSIZE_X + GUIBORDER_X + MULTIBUTTONSIZE_X),
	xMax = (-1 + MULTIBUTTONSIZE_X + GUIBORDER_X + (2 * MULTIBUTTONSIZE_X));

	buttonPos = glm::vec3((xMin + xMax) / 2, (yMin + yMax) / 2, 0);
	buttonDim = glm::vec2(xMax - xMin, yMax - yMin);
	text = new Texture("assets/textures/button_Load.png");
	butt = new Button(buttonPos, buttonDim, text, this->GUIShader);
	this->loadSaveButtons.push_back(pair<Button*, int>(butt, 2));

	curMinY = yMin;

	/*****************************************NUM CARS BUTTONS ********************************************/

	//Button 1 (Low)
	xMin = (-1 + (MULTIBUTTONSIZE_X/3) + GUIBORDER_X),
	yMin = (curMinY - GUIBORDER_Y) - MULTIBUTTONSIZE_Y,
	xMax = (-1 + (MULTIBUTTONSIZE_X / 3) + GUIBORDER_X) + MULTIBUTTONSIZE_X,
	yMax = (curMinY - GUIBORDER_Y);

	buttonPos = glm::vec3((xMin + xMax) / 2, (yMin + yMax) / 2, 0);
	buttonDim = glm::vec2(xMax - xMin, yMax - yMin);
	text = new Texture("assets/textures/button_Low.png");
	butt = new Button(buttonPos, buttonDim, text, this->GUIShader);
	this->numCarsButtons.push_back(pair<Button*, float>(butt, 0.125));

	//Button 2 (Med)
	xMin = (-1 + (MULTIBUTTONSIZE_X / 3) + GUIBORDER_X + MULTIBUTTONSIZE_X),
	xMax = (-1 + (MULTIBUTTONSIZE_X / 3) + GUIBORDER_X + (2 * MULTIBUTTONSIZE_X));

	buttonPos = glm::vec3((xMin + xMax) / 2, (yMin + yMax) / 2, 0);
	buttonDim = glm::vec2(xMax - xMin, yMax - yMin);
	text = new Texture("assets/textures/button_Med.png");
	butt = new Button(buttonPos, buttonDim, text, this->GUIShader);
	butt->isSelected = 2;
	this->numCarsButtons.push_back(pair<Button*, float>(butt, 0.25));

	//Button 3 (High)
	xMin = (-1 + (MULTIBUTTONSIZE_X / 3) + GUIBORDER_X + (2 * MULTIBUTTONSIZE_X)),
	xMax = (-1 + (MULTIBUTTONSIZE_X / 3) + GUIBORDER_X + (3 * MULTIBUTTONSIZE_X));

	buttonPos = glm::vec3((xMin + xMax) / 2, (yMin + yMax) / 2, 0);
	buttonDim = glm::vec2(xMax - xMin, yMax - yMin);
	text = new Texture("assets/textures/button_High.png");
	butt = new Button(buttonPos, buttonDim, text, this->GUIShader);
	this->numCarsButtons.push_back(pair<Button*, float>(butt, 0.5));

	curMinY = yMin;

	/*****************************************EMPTY TILE WEIGHTING********************************************/

	//Button 1 (Low)
	xMin = (-1 + (MULTIBUTTONSIZE_X / 3) + GUIBORDER_X),
	yMin = (curMinY - GUIBORDER_Y) - MULTIBUTTONSIZE_Y,
	xMax = (-1 + (MULTIBUTTONSIZE_X / 3) + GUIBORDER_X) + MULTIBUTTONSIZE_X,
	yMax = (curMinY - GUIBORDER_Y);

	buttonPos = glm::vec3((xMin + xMax) / 2, (yMin + yMax) / 2, 0);
	buttonDim = glm::vec2(xMax - xMin, yMax - yMin);
	text = new Texture("assets/textures/button_AVG.png");
	butt = new Button(buttonPos, buttonDim, text, this->GUIShader);
	this->emptyWeightButtons.push_back(pair<Button*, float>(butt, 0));

	//Button 2 (Med)
	xMin = (-1 + (MULTIBUTTONSIZE_X / 3) + GUIBORDER_X + MULTIBUTTONSIZE_X),
	xMax = (-1 + (MULTIBUTTONSIZE_X / 3) + GUIBORDER_X + (2 * MULTIBUTTONSIZE_X));

	buttonPos = glm::vec3((xMin + xMax) / 2, (yMin + yMax) / 2, 0);
	buttonDim = glm::vec2(xMax - xMin, yMax - yMin);
	text = new Texture("assets/textures/button_NAT.png");
	butt = new Button(buttonPos, buttonDim, text, this->GUIShader);
	butt->isSelected = 2;
	this->emptyWeightButtons.push_back(pair<Button*, float>(butt, 1));

	//Button 3 (High)
	xMin = (-1 + (MULTIBUTTONSIZE_X / 3) + GUIBORDER_X + (2 * MULTIBUTTONSIZE_X)),
	xMax = (-1 + (MULTIBUTTONSIZE_X / 3) + GUIBORDER_X + (3 * MULTIBUTTONSIZE_X));

	buttonPos = glm::vec3((xMin + xMax) / 2, (yMin + yMax) / 2, 0);
	buttonDim = glm::vec2(xMax - xMin, yMax - yMin);
	text = new Texture("assets/textures/button_CITY.png");
	butt = new Button(buttonPos, buttonDim, text, this->GUIShader);
	this->emptyWeightButtons.push_back(pair<Button*, float>(butt, 2));

	curMinY = yMin;



	/*****************************************START SIM BUTTON ********************************************/

	xMin = (-1 + MULTIBUTTONSIZE_X + GUIBORDER_X),
	yMin = (curMinY - GUIBORDER_Y) - SINGLEBUTTONSIZE_Y,
	xMax = (-1 + MULTIBUTTONSIZE_X + GUIBORDER_X) + SINGLEBUTTONSIZE_X,
	yMax = (curMinY - GUIBORDER_Y);

	curMinY = yMin;

	buttonPos = glm::vec3((xMin + xMax) / 2, (yMin + yMax) / 2, 0);
	buttonDim = glm::vec2(xMax - xMin, yMax - yMin);

	text = new Texture("assets/textures/button_StartSim.png");
	this->startSim = new Button(buttonPos, buttonDim, text, this->GUIShader);

	/*************************************************************SIMULATION BUTTONS ****************************************************************/

	/*****************************************END SIM BUTTON ********************************************/

	float curMaxY = -0.95;

	xMin = (-1 + MULTIBUTTONSIZE_X + GUIBORDER_X),
	yMin = (curMaxY + GUIBORDER_Y),
	xMax = (-1 + MULTIBUTTONSIZE_X + GUIBORDER_X) + SINGLEBUTTONSIZE_X,
	yMax = (curMaxY + GUIBORDER_Y) + SINGLEBUTTONSIZE_Y;

	curMaxY = yMax;

	buttonPos = glm::vec3((xMin + xMax) / 2, (yMin + yMax) / 2, 0);
	buttonDim = glm::vec2(xMax - xMin, yMax - yMin);

	text = new Texture("assets/textures/button_EndSim.png");
	this->endSim = new Button(buttonPos, buttonDim, text, this->GUIShader);

	/*****************************************HEATMAP BUTTON ********************************************/

	xMin = (-1 + MULTIBUTTONSIZE_X + GUIBORDER_X),
	yMin = (curMaxY + GUIBORDER_Y),
	xMax = (-1 + MULTIBUTTONSIZE_X + GUIBORDER_X) + SINGLEBUTTONSIZE_X,
	yMax = (curMaxY + GUIBORDER_Y) + SINGLEBUTTONSIZE_Y;

	curMaxY = yMax;

	buttonPos = glm::vec3((xMin + xMax) / 2, (yMin + yMax) / 2, 0);
	buttonDim = glm::vec2(xMax - xMin, yMax - yMin);

	text = new Texture("assets/textures/button_heatMap.png");
	this->heatmap = new Button(buttonPos, buttonDim, text, this->GUIShader);

}

void GUI::Draw(glm::vec4 normalisedCoords/*, KeyboardInput &k*/){

	// Enable blending and alpha
	glDepthMask(GL_FALSE);
	glEnable(GL_BLEND); 
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	this->GUIShader->Bind();
	this->GUIShader->UpdateGUI(0);

	//Draw GUI background
	textureGUI->Bind(0);
	glEnable(GL_TEXTURE_2D);
	this->background->Draw();


	if (!simRunning) {

		/*****************************************DRAW GRID SIZE BUTTONS ********************************************/
		for (pair<Button*, int> butt : gridSizeButtons) {
			Box test = butt.first->button->getAABB();
			//if button is selected, keep selected
			if (butt.first->isSelected == 2) {
				butt.first->Draw();
			}
			//if not selected check to see if hovering over if so then set
			else if (normalisedCoords.x < test.maxX && normalisedCoords.x > test.minX && normalisedCoords.y < test.maxY && normalisedCoords.y > test.minY)
			{
				butt.first->isSelected = 1;
				butt.first->Draw();
			}
			//if not hovering over and not selected then set to 0
			else
			{
				butt.first->isSelected = 0;
				butt.first->Draw();
			}
		}

		/*****************************************DRAW GEN MAP BUTTON ********************************************/

		Box test = this->genMap->button->getAABB();
		//if button is selected, keep selected
		if (this->genMap->isSelected == 2) {
			this->genMap->Draw();
		}
		//if not selected check to see if hovering over if so then set
		else if (normalisedCoords.x < test.maxX && normalisedCoords.x > test.minX && normalisedCoords.y < test.maxY && normalisedCoords.y > test.minY)
		{
			this->genMap->isSelected = 1;
			this->genMap->Draw();
		}
		//if not hovering over and not selected then set to 0
		else
		{
			this->genMap->isSelected = 0;
			this->genMap->Draw();
		}

		/*****************************************DRAW ROAD BUTTONS ********************************************/

		for (pair<Button*, int> butt : roadButtons) {
			Box test = butt.first->button->getAABB();
			//if button is selected, keep selected
			if (butt.first->isSelected == 2) {
				butt.first->Draw();
			}
			//if not selected check to see if hovering over if so then set
			else if (normalisedCoords.x < test.maxX && normalisedCoords.x > test.minX && normalisedCoords.y < test.maxY && normalisedCoords.y > test.minY)
			{
				butt.first->isSelected = 1;
				butt.first->Draw();
			}
			//if not hovering over and not selected then set to 0
			else
			{
				butt.first->isSelected = 0;
				butt.first->Draw();
			}
		}

		/*****************************************DRAW NUM CARS BUTTONS ********************************************/

		for (pair<Button*, float> butt : numCarsButtons) {
			Box test = butt.first->button->getAABB();
			//if button is selected, keep selected
			if (butt.first->isSelected == 2) {
				butt.first->Draw();
			}
			//if not selected check to see if hovering over if so then set
			else if (normalisedCoords.x < test.maxX && normalisedCoords.x > test.minX && normalisedCoords.y < test.maxY && normalisedCoords.y > test.minY)
			{
				butt.first->isSelected = 1;
				butt.first->Draw();
			}
			//if not hovering over and not selected then set to 0
			else
			{
				butt.first->isSelected = 0;
				butt.first->Draw();
			}
		}

		/*****************************************DRAW EMPTY WEIGHTING BUTTONS ********************************************/

		for (pair<Button*, int> butt : emptyWeightButtons) {
			Box test = butt.first->button->getAABB();
			//if button is selected, keep selected
			if (butt.first->isSelected == 2) {
				butt.first->Draw();
			}
			//if not selected check to see if hovering over if so then set
			else if (normalisedCoords.x < test.maxX && normalisedCoords.x > test.minX && normalisedCoords.y < test.maxY && normalisedCoords.y > test.minY)
			{
				butt.first->isSelected = 1;
				butt.first->Draw();
			}
			//if not hovering over and not selected then set to 0
			else
			{
				butt.first->isSelected = 0;
				butt.first->Draw();
			}
		}

		/*****************************************DRAW LOAD SAVE BUTTONS ********************************************/

		for (pair<Button*, int> butt : loadSaveButtons) {
			Box test = butt.first->button->getAABB();
			//if button is selected, keep selected
			if (butt.first->isSelected == 2) {
				butt.first->Draw();
			}
			//if not selected check to see if hovering over if so then set
			else if (normalisedCoords.x < test.maxX && normalisedCoords.x > test.minX && normalisedCoords.y < test.maxY && normalisedCoords.y > test.minY)
			{
				butt.first->isSelected = 1;
				butt.first->Draw();
			}
			//if not hovering over and not selected then set to 0
			else
			{
				butt.first->isSelected = 0;
				butt.first->Draw();
			}
		}

		/*****************************************DRAW START SIM BUTTON ********************************************/

		test = this->startSim->button->getAABB();
		//if button is selected, keep selected
		if (this->startSim->isSelected == 2) {
			this->startSim->Draw();
		}
		//if not selected check to see if hovering over if so then set
		else if (normalisedCoords.x < test.maxX && normalisedCoords.x > test.minX && normalisedCoords.y < test.maxY && normalisedCoords.y > test.minY)
		{
			this->startSim->isSelected = 1;
			this->startSim->Draw();
		}
		//if not hovering over and not selected then set to 0
		else
		{
			this->startSim->isSelected = 0;
			this->startSim->Draw();
		}

	}
	else {

		/*****************************************DRAW END SIM BUTTON ********************************************/
		Box test = this->endSim->button->getAABB();
		//if button is selected, keep selected
		if (this->endSim->isSelected == 2) {
			this->endSim->Draw();
		}
		//if not selected check to see if hovering over if so then set
		else if (normalisedCoords.x < test.maxX && normalisedCoords.x > test.minX && normalisedCoords.y < test.maxY && normalisedCoords.y > test.minY)
		{
			this->endSim->isSelected = 1;
			this->endSim->Draw();
		}
		//if not hovering over and not selected then set to 0
		else
		{
			this->endSim->isSelected = 0;
			this->endSim->Draw();
		}

		/*****************************************DRAW HEATMAP BUTTON ********************************************/

		test = this->heatmap->button->getAABB();
		//if button is selected, keep selected
		if (this->heatmap->isSelected == 2) {
			this->heatmap->Draw();
		}
		//if not selected check to see if hovering over if so then set
		else if (normalisedCoords.x < test.maxX && normalisedCoords.x > test.minX && normalisedCoords.y < test.maxY && normalisedCoords.y > test.minY)
		{
			this->heatmap->isSelected = 1;
			this->heatmap->Draw();
		}
		//if not hovering over and not selected then set to 0
		else
		{
			this->heatmap->isSelected = 0;
			this->heatmap->Draw();
		}

	}
		
	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);
}



void GUI::Update(KeyboardInput &k)
{
	
}

//Method that checks the buttons for collision with the mouse
void GUI::checkButtonCollision(glm::vec4 normalisedCoords){

	if (!simRunning) {

		/*****************************************CHECK GRID SIZE BUTTONS ********************************************/
		for (pair<Button*, int> butt : gridSizeButtons) {
			Box test = butt.first->button->getAABB();
			// Check for the mouse intersection with the buttons of the GUI
			if (normalisedCoords.x < test.maxX && normalisedCoords.x > test.minX && normalisedCoords.y < test.maxY && normalisedCoords.y > test.minY) {
				for (pair<Button*, int> butt2 : gridSizeButtons) {
					butt2.first->isSelected = 0;
				}
				butt.first->isSelected = 2;
				break;
			}
			/*else {
				butt.first->isSelected = 0;
			}*/
		}

		/*****************************************CHECK GEN MAP BUTTON ********************************************/
		Box test = this->genMap->button->getAABB();
		// Check for the mouse intersection with the buttons of the GUI
		if (normalisedCoords.x < test.maxX && normalisedCoords.x > test.minX && normalisedCoords.y < test.maxY && normalisedCoords.y > test.minY) {
			this->genMap->isSelected = 2;
		}
		else {
			this->genMap->isSelected = 0;
		}

		/*****************************************CHECK ROAD BUTTONS ********************************************/
		for (pair<Button*, int> butt : roadButtons) {
			Box test = butt.first->button->getAABB();
			// Check for the mouse intersection with the buttons of the GUI
			if (normalisedCoords.x < test.maxX && normalisedCoords.x > test.minX && normalisedCoords.y < test.maxY && normalisedCoords.y > test.minY) {
				roadPieceSelected = butt.second;
				butt.first->isSelected = 2;
			}
			else {
				butt.first->isSelected = 0;
			}
		}

		/*****************************************CHECK LOAD SAVE BUTTONS ********************************************/
		for (pair<Button*, int> butt : loadSaveButtons) {
			Box test = butt.first->button->getAABB();
			// Check for the mouse intersection with the buttons of the GUI
			if (normalisedCoords.x < test.maxX && normalisedCoords.x > test.minX && normalisedCoords.y < test.maxY && normalisedCoords.y > test.minY) {
				butt.first->isSelected = 2;
			}
			else {
				butt.first->isSelected = 0;
			}
		}

		/*****************************************CHECK NUM CARS BUTTONS ********************************************/

		for (pair<Button*, float> butt : numCarsButtons) {
			Box test = butt.first->button->getAABB();
			// Check for the mouse intersection with the buttons of the GUI
			if (normalisedCoords.x < test.maxX && normalisedCoords.x > test.minX && normalisedCoords.y < test.maxY && normalisedCoords.y > test.minY) {
				for (pair<Button*, int> butt2 : numCarsButtons) {
					butt2.first->isSelected = 0;
				}
				butt.first->isSelected = 2;
				break;
			}
		}

		/*****************************************CHECK EMPTY WEIGHTING BUTTONS ********************************************/
		for (pair<Button*, int> butt : emptyWeightButtons) {
			Box test = butt.first->button->getAABB();
			// Check for the mouse intersection with the buttons of the GUI
			if (normalisedCoords.x < test.maxX && normalisedCoords.x > test.minX && normalisedCoords.y < test.maxY && normalisedCoords.y > test.minY) {
				for (pair<Button*, int> butt2 : emptyWeightButtons) {
					butt2.first->isSelected = 0;
				}
				butt.first->isSelected = 2;
			}
			//else {
			//	butt.first->isSelected = 0;
			//}
		}

		/*****************************************CHECK START SIM BUTTON ********************************************/
		test = this->startSim->button->getAABB();
		// Check for the mouse intersection with the buttons of the GUI
		if (normalisedCoords.x < test.maxX && normalisedCoords.x > test.minX && normalisedCoords.y < test.maxY && normalisedCoords.y > test.minY) {
			this->startSim->isSelected = 2;
		}
		else {
			this->startSim->isSelected = 0;
		}

	}
	else {
		/*****************************************CHECK END SIM BUTTON ********************************************/
		Box test = this->endSim->button->getAABB();
		// Check for the mouse intersection with the buttons of the GUI
		if (normalisedCoords.x < test.maxX && normalisedCoords.x > test.minX && normalisedCoords.y < test.maxY && normalisedCoords.y > test.minY) {
			this->endSim->isSelected = 2;
		}
		else {
			this->endSim->isSelected = 0;
		}
		/*****************************************CHECK HEATMAP BUTTON ********************************************/
		test = this->heatmap->button->getAABB();
		// Check for the mouse intersection with the buttons of the GUI
		if (normalisedCoords.x < test.maxX && normalisedCoords.x > test.minX && normalisedCoords.y < test.maxY && normalisedCoords.y > test.minY) {
			if (this->heatmap->isSelected == 2) {	//if already selected
				this->heatmapOn = false;
				this->heatmap->isSelected = 0;
			}
			else {
				this->heatmapOn = true;
				this->heatmap->isSelected = 2;
			}
		}
	}
}

//Method that checks the GUI background for collision with the mouse
bool GUI::checkGUIcollision(glm::vec4 normalisedCoords){
	Box test = background->getAABB();
	// Check for the mouse intersection with the buttons of the GUI
	return (normalisedCoords.x < test.maxX && normalisedCoords.x > test.minX && normalisedCoords.y < test.maxY && normalisedCoords.y > test.minY);
}

int GUI::checkResizeButtons(){
	for (pair<Button*, int> butt : gridSizeButtons) {
		if (butt.first->isSelected == 2) {
			return butt.second;
		}
	}
	return -1;
}

bool GUI::checkGenMap(){
	bool newMap = false;
	if (this->genMap->isSelected == 2) {
		newMap = true;
		this->genMap->isSelected = 0;
	}
	return newMap;
}

int GUI::checkEmptyWeight(){
	for (pair<Button*, int> butt : emptyWeightButtons) {
		if (butt.first->isSelected == 2) {
			return butt.second;
		}
	}
	return 0;
}

bool GUI::checkSimStart(){
	if (this->startSim->isSelected == 2) {
		this->simRunning = true;
		this->startSim->isSelected = 0;
	}
	return this->simRunning;
}

bool GUI::checkSimEnd() {
	if (this->endSim->isSelected == 2) {
		this->simRunning = false;
		this->endSim->isSelected = 0;
	}
	return !this->simRunning; //return opp (simRunning is true -> endSim is false)
}

int GUI::checkSaveLoad()
{
	for (pair<Button*, int> butt : loadSaveButtons) {
		if (butt.first->isSelected == 2) {
			butt.first->isSelected = 0;
			return butt.second;
		}
	}
	return -1;
}
bool GUI::checkHeatmapStatus(){
	return this->heatmapOn;
}
/********************************
ADDED THIS BIT
*********************************/
void GUI::setGridSizeSelected(int size)
{
	for (pair<Button*, int> butt : gridSizeButtons) {
		if (butt.second == size) {
			butt.first->isSelected = 2;
		}
		else
			butt.first->isSelected = 0;
	}
	bool selected = false;
	for (pair<Button*, int> butt : gridSizeButtons) {
		if (butt.first->isSelected == 2)
			selected = true;
	}
	if (!selected) {
		gridSizeButtons.back().first->isSelected = 2;
	}
}
