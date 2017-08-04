#pragma once
#include <string>
#include "gl\glew.h"
#include "stb_image.h"
#include <cassert>
#include <iostream>
using namespace std;

class Texture
{
public:
	Texture() {};
	Texture(const string& file);

	void Bind(unsigned int numTexture);

	virtual ~Texture();
	GLuint texture;
private:
	Texture(const Texture& o) {} // Implement for copying
	void operator*(const Texture& o) {} 

	
protected:

};

