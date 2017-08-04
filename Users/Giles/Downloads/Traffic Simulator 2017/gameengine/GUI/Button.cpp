#include "Button.h"

Button::Button(glm::vec3 cen, glm::vec2 dim, Texture* tex, Shader* guiShader){

	this->pos[0] = glm::vec3(cen.x - dim.x/2, cen.y - dim.y/2, 0);
	this->pos[1] = glm::vec3(cen.x + dim.x/2, cen.y - dim.y/2, 0);
	this->pos[2] = glm::vec3(cen.x + dim.x/2, cen.y + dim.y/2, 0);
	this->pos[3] = glm::vec3(cen.x - dim.x/2, cen.y + dim.y/2, 0);

	this->tex[0] = glm::vec2(0, 1);
	this->tex[1] = glm::vec2(1, 1);
	this->tex[2] = glm::vec2(1, 0);
	this->tex[3] = glm::vec2(0, 0);

	this->normal[0] = glm::vec3(0.0, 0.0, -1.0);
	this->normal[1] = glm::vec3(0.0, 0.0, -1.0);
	this->normal[2] = glm::vec3(0.0, 0.0, -1.0);
	this->normal[3] = glm::vec3(0.0, 0.0, -1.0);

	unsigned int buttonIndices[] = { 0,1,2,0,2,3 };

	this->buttonVerts = new Vert [4]
	{
		Vert(this->pos[0], this->tex[0], this->normal[0]),
		Vert(this->pos[1], this->tex[1], this->normal[1]),
		Vert(this->pos[2], this->tex[2], this->normal[2]),
		Vert(this->pos[3], this->tex[3], this->normal[3])

	};

	this->button = new Mesh(buttonVerts, sizeof(buttonVerts), buttonIndices, sizeof(buttonIndices));
	this->texture = tex;

	this->isSelected = 0;
	this->GUIShader = guiShader;

}

void Button::Draw(){
	this->GUIShader->Bind();
	this->GUIShader->UpdateGUI(this->isSelected);
	this->texture->Bind(0);

	this->button->Draw();
}

void Button::Update(){



}
