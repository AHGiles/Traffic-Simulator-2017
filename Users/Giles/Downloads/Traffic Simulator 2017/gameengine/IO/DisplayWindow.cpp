#include "DisplayWindow.h"



DisplayWindow::DisplayWindow(){
	string name("Default");
	int width = 800;
	int height = 600;

	SDL_Init(SDL_INIT_VIDEO); // Initialise SDL


	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8); // Initialise SDL colour bit attributes
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16); // Setup zBuffer depth
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32); // Initialise SDL buffer value
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); // Initialise a double buffer


	main_Window = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL /*| SDL_WINDOW_FULLSCREEN | SDL_WINDOW_INPUT_GRABBED*/);
	main_glContext = SDL_GL_CreateContext(main_Window);

	GLenum windowStatus = glewInit(); // Initialise glew

	if (windowStatus != GLEW_OK) // Check for glew errors
		cerr << "Glew failed to initialise in DisplayWindow.cpp." << endl;

	finished = false; // Make sure we won't terminate instantly


					  // Enable backface culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	// Enable zBuffer
	glEnable(GL_DEPTH_TEST);
}

DisplayWindow::DisplayWindow(int width, int height, const string& name)
{
	SDL_Init(SDL_INIT_VIDEO); // Initialise SDL


	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8); // Initialise SDL colour bit attributes
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16); // Setup zBuffer depth
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32); // Initialise SDL buffer value
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); // Initialise a double buffer


	main_Window = SDL_CreateWindow(name.c_str(),SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL /*SDL_WINDOW_FULLSCREEN |*/);
	main_glContext = SDL_GL_CreateContext(main_Window);

	GLenum windowStatus = glewInit(); // Initialise glew

	if (windowStatus != GLEW_OK) // Check for glew errors
		cerr << "Glew failed to initialise in DisplayWindow.cpp." << endl;

	cout << "GLEW version: " << glewGetString(GLEW_VERSION) << endl;

	//checking the version
	int OpenGLVersion[2];
	glGetIntegerv(GL_MAJOR_VERSION, &OpenGLVersion[0]);
	glGetIntegerv(GL_MINOR_VERSION, &OpenGLVersion[1]);

	cout << "Open GL Version: " << OpenGLVersion[0] << "." << OpenGLVersion[1] << endl;

	finished = false; // Make sure we won't terminate instantly


	// Enable backface culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	// Enable zBuffer
	glEnable(GL_DEPTH_TEST);
}

DisplayWindow::~DisplayWindow()
{
	SDL_GL_DeleteContext(main_glContext);
	SDL_DestroyWindow(main_Window);
	SDL_Quit();
}

void DisplayWindow::Update()
{
	SDL_GL_SwapWindow(main_Window); // Swap buffer window

	SDL_Event e;

	while (SDL_PollEvent(&e)) // Check for termination of program
	{
		cout << "in display while loop" << endl;
		if (e.type == SDL_QUIT)
			finished = true;
	}

}

bool DisplayWindow::isFinished()
{
	return finished;
}

void DisplayWindow::ClearColour(float a, float b, float c, float d)
{
	glClearColor(a,b,c,d);
	glClear(GL_COLOR_BUFFER_BIT); // Clear the colour buffer
	glClear(GL_DEPTH_BUFFER_BIT); // Clear the zBuffer


}

void DisplayWindow::setWindowTitle(const char * name)
{

	SDL_SetWindowTitle(this->main_Window, name);
}

void DisplayWindow::loadLoading(){

	main_surface = SDL_GetWindowSurface(main_Window);
	for (size_t i = 0; i < 4; i++)
	{
		string file = "assets\\loading" + to_string(i) + ".bmp";
		SDL_Surface* temp = SDL_LoadBMP(file.c_str());
		loadingImages.push_back(temp);
	}
	
	//loadingImage = SDL_LoadBMP(file.c_str());


	SDL_BlitScaled(loadingImages.at(0), NULL, main_surface, NULL);
	SDL_UpdateWindowSurface(main_Window);

}

void DisplayWindow::closeLoading(){

	SDL_FreeSurface(main_surface);
	for (size_t i = 0; i < loadingImages.size(); i++){
		SDL_FreeSurface(loadingImages.at(i));
	}
}

