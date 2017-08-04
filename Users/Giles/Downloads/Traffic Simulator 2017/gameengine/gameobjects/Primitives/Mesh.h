#pragma once

#include <gl/glew.h>
#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include "Box.h"
// 3rd party loader from GitHub - Must be a singular mesh loaded in (not multiple meshes in one file), must have vertex positions, UVs and normals.
#include "../ObjectLoading/obj_loader.h" 
#include "../GameObjects/Transform.h"
#include "../OpenGLHelper.h"
#include "../../lib/Assimp/include/assimp/Importer.hpp"
//#include <assimp/Importer.hpp>

#include "../../lib/Assimp/include/assimp/scene.h"
//#include <assimp/scene.h>

#include "../../lib/Assimp/include/assimp/postprocess.h"
//#include <assimp/postprocess.h>

struct Vert
{
public:
	Vert(const glm::vec3& pos, const glm::vec2 tex, const glm::vec3 norm)
	{
		this->pos = pos;
		this->tex = tex;
		this->normal = norm;
	}

	inline glm::vec3* getPos() { return &pos; }
	inline glm::vec2* getTex() { return &tex; }
	inline glm::vec3* getNormal() { return &normal; }
private:
	glm::vec3 pos;
	glm::vec2 tex;
	glm::vec3 normal;
	
};

class Mesh
{
public:
	Mesh() {}
	Mesh(Vert* verts, unsigned int numVerts, unsigned int* indices, unsigned int numIndices);
	Mesh(const std::string file);

	void Draw();
	void Init(const IndexedModel& model);

	

	virtual ~Mesh();

	inline Box getAABB() { return this->AABB; }
	bool AABBCollision(glm::vec3 point);
private:
	Mesh(const Mesh& o);
	void operator*(const Mesh& o);

	enum
	{	
		POSITION_VB,
		TEXTURE_VB,
		NORMAL_VB,
		INDEX_VB,
	};

	Box AABB;
	static const unsigned int NUM_BUFFERS = 4;
	GLuint VAO;
	GLuint VAB[NUM_BUFFERS];

	unsigned int numIndices;
	unsigned int drawCount;
	unsigned int numVerts;


protected:


};

