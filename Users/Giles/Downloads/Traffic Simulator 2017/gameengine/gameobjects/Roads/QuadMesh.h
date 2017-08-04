#pragma once

#include "../Primitives/QuadMeshInfo.h"
using namespace std;

class QuadMesh
{
public:

	QuadMesh() {};
	QuadMesh(QuadMeshInfo* q);
	QuadMesh(Model* a);
	QuadMesh(Model* a, Model* b, Model* c, Model* d);
	AssimpModel* quads[4];
	QuadMeshInfo* info;

	bool genQuads();
	void Draw();



};