#pragma once
using namespace std;
#include <string>
#include <iostream>
#include <SDL2\SDL.h>
#include <gl\glew.h>
#include <vector>

class DisplayWindow
{
public:
	DisplayWindow();
	DisplayWindow(int width, int height, const string& name);
	virtual ~DisplayWindow();
	void Update();
	bool isFinished();
	void ClearColour(float a, float b, float c, float d);

	void setWindowTitle(const char* name);
//private:
	DisplayWindow(const DisplayWindow& o) {}
	void operator*(const DisplayWindow& o) {}

	SDL_Window* main_Window;
	SDL_GLContext main_glContext;
	SDL_Surface* main_surface, *loadingImage;
	std::vector<SDL_Surface*> loadingImages;

	void loadLoading();
	void closeLoading();

	bool finished;
protected:
};

