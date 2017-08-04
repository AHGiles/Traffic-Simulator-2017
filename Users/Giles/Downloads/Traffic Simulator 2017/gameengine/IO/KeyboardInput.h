#pragma once
#include "../GameObjects/Camera.h"
#include <SDL2\SDL.h>


class KeyboardInput
{
public:
	KeyboardInput(bool i) { this->acceptingInputs = i; this->mouseClicked = false; mouseClickCount = 0; }

	bool Process(Camera& camera, SDL_Event& event, const Uint8* keyState);
	void Update(SDL_Event& event, const Uint8* keyState);
	bool GUIInteraction();
	void inline flipInputs() { this->acceptingInputs = !this->acceptingInputs; }
private:
	bool acceptingInputs;

	bool mouseClicked; int mouseClickCount;

	SDL_Event event;
	const Uint8 *keystate = SDL_GetKeyboardState(NULL);
};

