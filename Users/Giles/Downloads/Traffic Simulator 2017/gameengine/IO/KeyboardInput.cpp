#include "KeyboardInput.h"

#include<iostream>
using namespace std;


bool KeyboardInput::Process(Camera& camera, SDL_Event& event, const Uint8* keyState)
{
	this->event = event;
	this->keystate = keyState;
	bool retVal = false;
	if (acceptingInputs)
	{
			this->event.key.keysym.sym;
			
		
			/* Keyboard event */
			if (keyState[SDL_SCANCODE_W])
				camera.moveForward(10.0f);

			if (keyState[SDL_SCANCODE_S])
				camera.moveBack(10.0f);

			if (keyState[SDL_SCANCODE_A])
				camera.moveRight(1.0f);

			if (keyState[SDL_SCANCODE_D])
				camera.moveLeft(1.0f);

			if (keyState[SDL_SCANCODE_Q])
				camera.yaw(1.0f);

			if (this->keystate[SDL_SCANCODE_E])
				camera.yaw(-1.0f);

			if(this->keystate[SDL_SCANCODE_G])


			if (this->keystate[SDL_SCANCODE_ESCAPE])
				retVal = true;
			
		
	}
	return retVal;
	
}

void KeyboardInput::Update(SDL_Event& event, const Uint8* keyState)
{
	this->event = event;
	this->keystate = keyState;
	if (mouseClickCount > 0)
		mouseClickCount--;
		
}

bool KeyboardInput::GUIInteraction()
{
	
		if (acceptingInputs && this->keystate[SDL_MOUSEBUTTONDOWN] && mouseClickCount==0)
		{
			switch (this->event.button.button)
			case SDL_BUTTON_LEFT:
				mouseClicked = true;
				mouseClickCount = 5;
		}
		else
			this->mouseClicked = false;
	
	return mouseClicked;

}


