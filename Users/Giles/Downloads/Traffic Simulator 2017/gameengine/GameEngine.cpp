#include "headers.h"

#define WIDTH 1200
#define HEIGHT 800

using namespace std;

//Program Variables
int mouseX = 0, mouseY = 0;
glm::vec4 normalisedCoords;

glm::mat4 ProjectionMatrix;

DisplayWindow* display;

World2D world2D;
World3D world3D;

Camera camera;

//Simulation variables
bool simRunning = false;
ModelLibrary* models;

//DeltaT using c++
float previousTime;
int frame_accum = 0;
float startTime = 0.0f;
int fps = 0;

//Protyping methods
void keyboardProcess(double deltaT);
void mouseProcess(double deltaT);

void threadedLoadingScreen(DisplayWindow* display, bool* loadedModels);

void startSimulation();
void endSimulation();

KeyboardInput keyboard(true);
SDL_Event event;
const Uint8* keyState = SDL_GetKeyboardState(NULL);
bool isFinished = false, mouseDown = false;

//timing voodoo
float programTime = 0;
float drawTime = 0;
float updateTime = 0;
float SDLTime = 0;

void Reshape(int width, int height) {

	glViewport(0, 0, width, height);						// Reset The Current Viewport

															//Set the projection matrix
	ProjectionMatrix = glm::perspective(70.0f, (GLfloat)width / (GLfloat)height, 0.1f, 10000.0f);
}


void Init() {
	camera = Camera(glm::vec3((INITGRIDSIZE / 3) * TILESIZE, 1100.0f, (INITGRIDSIZE / 2) * TILESIZE), &ProjectionMatrix);

	
	//load loading screen pic/s
	display->loadLoading();
	
	bool loadedModels = false;
	thread thread1(threadedLoadingScreen, display, &loadedModels);

	models = new ModelLibrary();
	
	loadedModels = true;
	thread1.join();

	display->closeLoading();

	world2D = World2D(models);
	world3D = World3D(models);
	world2D.Init();
}

void threadedLoadingScreen(DisplayWindow* display, bool* loadedModels) {
	//printf("in thread\n");
	int counter = 0;
	while (!*loadedModels) {
		//printf("in thread\n");

		float currentTime = GetTickCount();

		if (counter >= 240)
		{
			counter = 0;
		}
		else {
			if (counter >= 180) {
				SDL_BlitScaled(display->loadingImages.at(3), NULL, display->main_surface, NULL);
				//render loading screen indefinitly 
				SDL_UpdateWindowSurface(display->main_Window);
				counter++;
			}
			else {
				if (counter >= 120) {
					SDL_BlitScaled(display->loadingImages.at(2), NULL, display->main_surface, NULL);
					//render loading screen indefinitly 
					SDL_UpdateWindowSurface(display->main_Window);
					counter++;
				}
				else {
					if (counter >= 60) {
						SDL_BlitScaled(display->loadingImages.at(1), NULL, display->main_surface, NULL);
						//render loading screen indefinitly 
						SDL_UpdateWindowSurface(display->main_Window);
						counter++;
					}
					else {
						SDL_BlitScaled(display->loadingImages.at(0), NULL, display->main_surface, NULL);
						//render loading screen indefinitly 
						SDL_UpdateWindowSurface(display->main_Window);
						counter++;
					}
				}
			}
		}
		
	}
}

void Draw() {
	

	if (!simRunning) {
		display->ClearColour(0.9f, 0.9f, 0.9f, 1.0f); // Set clear colour
		world2D.Draw(normalisedCoords, camera);
	}
	else {
		display->ClearColour(0.18f, 0.34f, 0.74f, 1.0f); // Set clear colour
		//display->ClearColour(0.82f, 0.76f, 0.23f, 1.0f); // Set clear colour
		world3D.Draw(normalisedCoords, camera);
	}
}
void Update(double deltaT) {
	// Obtain mouse coordinates
	SDL_GetMouseState(&mouseX, &mouseY);

	//Get the screen coordinates of the mouse
	normalisedCoords = glm::vec4(
		(2.0f*mouseX) / WIDTH - 1.0f,	// x
		1.0f - (2.0f*mouseY) / HEIGHT,  // y
		0.5f,							// z
		1.0f							// w
	);


	if (!simRunning) {
		world2D.Update(deltaT);
		world2D.Raytrace(HEIGHT, WIDTH, mouseX, mouseY, &camera);
		//check to see if start sim has been pressed
		if (world2D.gui->checkSimStart()) {
			world2D.invalidCells = world2D.validateMap();
			if (world2D.invalidCells.size() == 0)
				startSimulation();
			else {
				world2D.gui->simRunning = false;
			}
		}
	}
	else {
		world3D.Update(deltaT);
		world3D.Raytrace(HEIGHT, WIDTH, mouseX, mouseY, &camera);
		//check to see if end sim has been pressed
		if (world3D.gui->checkSimEnd()) {
			endSimulation();
		}
	}

}

void startSimulation() {
	//world3D = World3D(models);
	world3D.Init(world2D.roadGrid, world2D.roadMap, world2D.gridSize, world2D.tileSize, world2D.gui, world2D.roadShader);
	//create 3dview camera with a more RTS esque view
	camera.changeCam(glm::vec3((world2D.gridSize / 2) * TILESIZE, (TILESIZE * 5), (world2D.gridSize / 2) * TILESIZE), glm::vec3(0, -0.7, -0.3), glm::cross(glm::vec3(1, 0, 0), glm::vec3(0, -0.7, -0.3)));
	simRunning = true;
}

void endSimulation() {
	//delete world3D
	world3D.endSim();
	//world2D.Init();

	//create 2dview camera with a less RTS esque view
	camera.changeCam(glm::vec3((world2D.gridSize / 3) * TILESIZE, 1100.0f, (world2D.gridSize / 2) * TILESIZE), glm::vec3(0.0, -0.1, -0.0), glm::vec3(0.0, 0.0, -1.0));
	simRunning = false;
}

int main(int argc, char** argv)
{
	// Setup OpenGL
	SDL_PumpEvents();
	//SDL_ShowCursor(0); // Hide mouse
	//SDL_SetRelativeMouseMode(SDL_TRUE); // Keep track of mouse and keep inside of window
	display = new DisplayWindow(WIDTH, HEIGHT, "Game Engine"); // Create window

	Init();

	while (!isFinished)
	{
		//programTime = GetTickCount();
		//**************************DELTAT USING C++*********************************
		// Calculate delta time
		float currentTime = GetTickCount();
		float deltaT = (currentTime - previousTime) / 1000.0f;
		previousTime = currentTime;

		frame_accum++;
		if (currentTime - startTime >= 1000.0f)
		{
			stringstream title;
			title << "Game Engine - FPS [" << frame_accum << "]";
			display->setWindowTitle(title.str().c_str());
			startTime = currentTime;
			frame_accum = 0;
		}

		Reshape(WIDTH, HEIGHT);

		//float tempTime = GetTickCount();
		Update(deltaT);
		//updateTime = GetTickCount() - tempTime;

		//tempTime = GetTickCount();
		Draw();
		//drawTime = GetTickCount() - tempTime;
		// Update display
		//display->Update();	

		glFlush();

		//tempTime = GetTickCount();
		//Mouse interaction
		SDL_GL_SwapWindow(display->main_Window); // Swap buffer window
		//SDL_Event e;
		while (SDL_PollEvent(&event)) //Check for mouse events
		{
			mouseProcess(deltaT);
			keyboardProcess(deltaT);
		}
		//SDLTime = GetTickCount() - tempTime;
		
		//cout << "Update Time: " << updateTime << endl;
		//cout << "Draw Time: " << drawTime << endl;
		//cout << "SDL Time: " << SDLTime << endl;
		//cout << "Program Time: " << GetTickCount() - programTime << "\n\n" << endl;
		// Check to see if we've finished
		//isFinished = keyboard.Process(camera, event, keyState);

	}
	return 0;
}

void keyboardProcess(double deltaT){
		event.key.keysym.sym;

		/* Keyboard event */

		//Camera Controls
		glm::vec2 camMoveDir;
		int camRotDir = 0;

		if (keyState[SDL_SCANCODE_W])
			camMoveDir.y = 1;

		if (keyState[SDL_SCANCODE_S])
			camMoveDir.y = -1;

		if (keyState[SDL_SCANCODE_A])
			camMoveDir.x = -1;

		if (keyState[SDL_SCANCODE_D])
			camMoveDir.x = 1;

		if (keyState[SDL_SCANCODE_Q])
			camRotDir = -1;

		if (keyState[SDL_SCANCODE_E])
			camRotDir = 1;

		camera.moveCam(camMoveDir, 1);
		camera.rotateCam(camRotDir, 1);

		//Program Controls
		if (keyState[SDL_SCANCODE_G])
			world2D.genMap();

		if (keyState[SDL_SCANCODE_ESCAPE])
			isFinished = true;

		if (keyState[SDL_SCANCODE_SPACE]) {
				
		}
		if (simRunning) {
			if (keyState[SDL_SCANCODE_1]) {
				camera.changeCam(glm::vec3(camera.getPos().x, (TILESIZE * 5), camera.getPos().z), glm::vec3(0, -0.7, -0.3), glm::cross(glm::vec3(1, 0, 0), glm::vec3(0, -0.7, -0.3)));
				world3D.carCam = false;
			}
			if (keyState[SDL_SCANCODE_2]) {
				camera.changeCam(glm::vec3(camera.getPos().x, (15), camera.getPos().z), glm::vec3(0, 0, -1.0), glm::vec3(0, 1, 0));
				world3D.carCam = false;
			}
			if (keyState[SDL_SCANCODE_C]) {
				world3D.carCam = !world3D.carCam;
			}
			if (keyState[SDL_SCANCODE_LEFT]) {
				world3D.prevActiveCar();
			}
			if (keyState[SDL_SCANCODE_RIGHT]) {
				world3D.nextActiveCar();
			}
		}

}

void mouseProcess(double deltaT)
{

	//cout << world2D.gui->ifSelected << endl;
	switch (event.type) 
	{

		case SDL_MOUSEWHEEL:
		{
			int camZoomDir = 0;

			if (event.wheel.y < 0) {
				camZoomDir = -1;
			}
			else {
				camZoomDir = 1;
			}
			camera.zoomCam(camZoomDir, 1);
		}
		break;

		case SDL_MOUSEBUTTONDOWN:
		{
			//if mouse is inside GUI
			if (world2D.gui->checkGUIcollision(normalisedCoords)) {
				//check button collisions
				world2D.gui->checkButtonCollision(normalisedCoords);

			}
			//if mouse isnt it GUI
			else {
				//if sim isnt running
				if (!simRunning) {
					//and a button has been selected previously
					if (world2D.gui->roadPieceSelected != -1) {

						//check each road cell and add respective road piece to correct cell
						for (size_t i = 0; i < world2D.gridSize; i++)
						{
							for (size_t j = 0; j < world2D.gridSize; j++)
							{
								if (world2D.roadGrid[i][j]->isSelected == 1)
								{
									world2D.addRoadPiece(glm::vec2(i, j), world2D.gui->roadPieceSelected);
								}
							}
						}
					}
				}
				//if sim is running
				else {
					for (size_t i = 0; i < world2D.gridSize; i++)
					{
						for (size_t j = 0; j < world2D.gridSize; j++)
						{
							if (world3D.roadGrid[i][j]->isSelected == 1)
							{
								cout << world3D.roadGrid[i][j]->toString(world3D.simTime) << endl;
							}
						}
					}
				}
			}
			
			break;
		}

		case SDL_QUIT:
		{
			isFinished = true;
		}
		break;

		
		}

}






