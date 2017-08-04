#pragma once
#include "Mesh.h"
#include "../ObjectLoading/Texture.h"
#include <string>
using namespace std;

struct Model 
{
public:

	Model() {}
	Model(string a, string b)
	{
		mesh = new Mesh(a);
		tex = new Texture(b);
	}
	Mesh* mesh;
	Texture* tex;
};