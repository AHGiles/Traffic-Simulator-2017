#include "Road.h"

GLfloat mat_Shininess = 50;

void Road::Draw(Shader* roadShader, Camera cam) {

	roadShader->Bind();

	AssimpModel* tile = *this->model;
	tile->BindModelsVAO();

	//this->model->BindModelsVAO();
	
	if (this->roadType >= TJUNC && !this->activeTraffic.empty()) {
		roadShader->UpdateRoad(this->transform, cam, this->isSelected, this->activeTraffic.front(), 50.0f);
	}else
		roadShader->UpdateRoad(this->transform, cam, this->isSelected, -1, 100.0f);

	// Enable textures
	glEnable(GL_TEXTURE_2D);
	//this->model->RenderModel();
	tile->RenderModel();
}
void Road::DrawHeatMap(Shader * heatmapShader, Camera cam, int simTime){

	heatmapShader->Bind();

	AssimpModel* tile = *this->model;
	tile->BindModelsVAO();

	//this->model->BindModelsVAO();

	if (this->roadType > EMPTY) {
		float heatmapStat = this->timeOccupiedByCar / (float)simTime;
		//if (heatmapStat > 0) {
		//	cout << heatmapStat << endl;
		//}
		heatmapShader->UpdateRoadHeatMap(this->transform, cam, heatmapStat);
	}
	else
		heatmapShader->UpdateRoadHeatMap(this->transform, cam, -1.0f);

	// Enable textures
	glEnable(GL_TEXTURE_2D);
	//this->model->RenderModel();
	tile->RenderModel();
}
void Road::setModel(AssimpModel ** newModel){
	this->model = newModel;
}
string Road::toString(int simTime){
	string str = "*************** CELL (" + to_string((int)this->cellPos.x) + "," + to_string((int)this->cellPos.y) + ") ***************\n";

	str.append("Total Sim Time: \t" + to_string(simTime) + "\n");

	str.append("Road Piece Type: \t" + getRoadType() + "\n");

	str.append("Time Road Occupied: \t" + to_string(this->timeOccupiedByCar) + "\n");

	str.append("Occupied Percentage: \t" + to_string((int)((this->timeOccupiedByCar / simTime) * 100)) + "%\n");
	
	str.append("Times Traversed: \t" + to_string(timesTraversed) + "\n");

	str.append("Most Active Light: \t" + getMostActiveLight() + "\n");


	return str;
}
string Road::getRoadType(){
	switch (this->roadType) {
	case EMPTY:
		return "Empty";
		break;
	case STRAIGHT:
		return "Straight";
		break;
	case CORNER:
		return "Corner";
		break;
	case TJUNC:
		return "T-Junction";
		break;
	case CROSSROADS:
		return "Crossroads";
		break;
	default:
		return "Empty";
		break;
	}
}

string Road::getMostActiveLight()
{
	switch (this->roadType) {
	case EMPTY:
		return "N/A";
		break;
	case STRAIGHT:
		return "N/A";
		break;
	case CORNER:
		return "N/A";
		break;
	case TJUNC:
	case CROSSROADS:
		int curMax = 0;
		int bestDir = -1;
		for (int i = 0; i < 4; i++) {
			if (this->lights[i] > curMax) {
				curMax = this->lights[i];
				bestDir = i;
			}
		}
		switch (bestDir) {
		case 0:
			return "North";
			break;
		case 1:
			return "East";
			break;
		case 2:
			return "South";
			break;
		case 3:
			return "West";
			break;
		}
		break;
	}
}
;