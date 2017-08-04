#pragma once
#include "../GameObjects/Primitives/Mesh.h"
#include "../ObjectLoading/Texture.h"
#include "../Shader/Shader.h"


class Button {

public:
	glm::vec3 pos[4];
	glm::vec2 tex[4];
	glm::vec3 normal[4];
	Vert* buttonVerts;

	Mesh* button;
	Texture* texture;

	Shader* GUIShader;

	int isSelected;

	Button(glm::vec3 cen, glm::vec2 dim, Texture* tex, Shader* guiShader);

	void Draw();

	void Update();

};
