#pragma once
using namespace std;
#include<string>
#include<gl\glew.h>
#include "../GameObjects/Transform.h"
#include "../GameObjects/Camera.h"
#include "../OpenGLHelper.h"

class Shader
{
public:
	Shader() {}
	Shader(const string& fileName);

	void Bind();
	void Update(Transform transform, Camera camera);
	void UpdateRoad(Transform transform, Camera camera, int isSelected, int activeLight, GLfloat mat_shininess);
	void UpdateRoadHeatMap(Transform transform, Camera camera, float heatMapStat);
	void UpdateCubeMap(Camera camera);
	void UpdateGUI(int isSelected);

	virtual ~Shader();

	GLuint getTextureUniform() { return this->main_uniforms[DIFFUSE_MAP]; }

private:
	static const unsigned int NUM_SHADERS = 2;
	Shader(const Shader& o) {}
	void operator*(const Shader& o) {}

	static bool CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const string& errorMessage);
	static string LoadShader(const string& fileName);
	static GLuint CreateShader(const string& input, GLenum type);

	enum
	{
		TRANSFORM_U,

		MODEL,

		NORMAL_U,

		DIFFUSE_MAP,

		LIGHT_DIFFUSE,

		LIGHT_AMB,

		LIGHT_SPEC,

		LIGHT_DIR,

		MAT_SHIN,

		SELECTED,

		ACTIVE_LIGHT,

		HEATMAP_STAT,

		NUM_UNIFORMS
	};

	GLuint main_program;
	GLuint main_shaders[NUM_SHADERS];
	GLuint main_uniforms[NUM_UNIFORMS];

protected:

};

