#include "ModelLibrary.h"

ModelLibrary::ModelLibrary(){

	twoD = true;

	//Emptys
	this->empty2D.LoadModelFromFile("assets/models/Empty.obj");
	this->empty2D.FinalizeVBO();

	//Empties
	AssimpModel* temp = new AssimpModel(); // 0
	temp->LoadModelFromFile("assets/models/Flowers.obj");
	temp->FinalizeVBO();
	this->empties3D.push_back(temp);

	temp = new AssimpModel(); // 1
	temp->LoadModelFromFile("assets/models/Pool.obj");
	temp->FinalizeVBO();
	this->empties3D.push_back(temp);

	temp = new AssimpModel(); // 2
	temp->LoadModelFromFile("assets/models/EmptyTree.obj");
	temp->FinalizeVBO();
	this->empties3D.push_back(temp);

	temp = new AssimpModel(); // 3
	temp->LoadModelFromFile("assets/models/EmptyHouses.obj");
	temp->FinalizeVBO();
	this->empties3D.push_back(temp);

	temp = new AssimpModel(); // 4
	temp->LoadModelFromFile("assets/models/EmptyHousesRotated.obj");
	temp->FinalizeVBO();
	this->empties3D.push_back(temp);

	temp = new AssimpModel(); // 5
	temp->LoadModelFromFile("assets/models/EmptyHouses2.obj");
	temp->FinalizeVBO();
	this->empties3D.push_back(temp);

	temp = new AssimpModel(); // 6
	temp->LoadModelFromFile("assets/models/EmptyHousesRotated2.obj");
	temp->FinalizeVBO();
	this->empties3D.push_back(temp);

	temp = new AssimpModel(); // 7
	temp->LoadModelFromFile("assets/models/EmptyTree2.obj");
	temp->FinalizeVBO();
	this->empties3D.push_back(temp);

	this->empties3D.push_back(&empty2D); // 8

	for (int i = 0; i < 9; i++) {
		this->curEmpties.push_back(&empty2D);
	}

	//Straights
	this->straight2D.LoadModelFromFile("assets/models/Straight.obj");
	this->straight2D.FinalizeVBO();
	this->curStraight = &this->straight2D;

	//Corner
	this->corner2D.LoadModelFromFile("assets/models/Corner.obj");
	this->corner2D.FinalizeVBO();
	this->curCor = &this->corner2D;

	//TJuncs
	this->tJunc2D.LoadModelFromFile("assets/models/TJunc.obj");
	this->tJunc2D.FinalizeVBO();
	this->tJunc3D.LoadModelFromFile("assets/models/TJunc_traffic.obj");
	this->tJunc3D.FinalizeVBO();
	this->curT = &this->tJunc2D;

	//Crossroads
	this->cross2D.LoadModelFromFile("assets/models/Crossroads.obj");
	this->cross2D.FinalizeVBO();
	this->cross3D.LoadModelFromFile("assets/models/CrossroadsTraffic.obj");
	this->cross3D.FinalizeVBO();
	this->curCross = &this->cross2D;

	//Cars
	temp = new AssimpModel();
	temp->LoadModelFromFile("assets/models/BMW.obj");
	temp->FinalizeVBO();
	this->cars.push_back(temp);

	temp = new AssimpModel();
	temp->LoadModelFromFile("assets/models/Muscle.obj");
	temp->FinalizeVBO();
	this->cars.push_back(temp);

	temp = new AssimpModel();
	temp->LoadModelFromFile("assets/models/Car.obj");
	temp->FinalizeVBO();
	this->cars.push_back(temp);


}

void ModelLibrary::swapModels(){
	swapEmpty();
	//swapStraight();
	//swapCorner();
	swapCross();
	swapTJunc();
	twoD = !twoD;
}

void ModelLibrary::swapEmpty(){
	if (twoD) {
		for (int i = 0; i < curEmpties.size(); i++) {
			curEmpties.at(i) = empties3D.at(i);
		}
	}
	else {
		for (int i = 0; i < curEmpties.size(); i++) {
			curEmpties.at(i) = &empty2D;
		}
	}
}

void ModelLibrary::swapStraight(){
	if (twoD) {
		this->curStraight = &this->straight3D;
	}
	else {
		this->curStraight = &this->straight2D;
	}
}

void ModelLibrary::swapCorner(){
	if (twoD) {
		this->curCor = &this->corner3D;
	}
	else {
		this->curCor = &this->corner2D;
	}
}

void ModelLibrary::swapTJunc(){
	if (twoD) {
		this->curT = &this->tJunc3D;
	}
	else {
		this->curT = &this->tJunc2D;
	}
}

void ModelLibrary::swapCross(){
	if (twoD) {
		this->curCross = &this->cross3D;
	}
	else {
		this->curCross = &this->cross2D;
	}
}

AssimpModel** ModelLibrary::getStraight(){
	return &this->curStraight;
}

AssimpModel** ModelLibrary::getCorner(){
	return &this->curCor;
}

AssimpModel** ModelLibrary::getTJunc(){
	return &this->curT;
}

AssimpModel** ModelLibrary::getCross(){
	return &this->curCross;
}

AssimpModel * ModelLibrary::getACar(){
	int numCars = this->cars.size();

	if (numCars > 1) {
		//srand(time(NULL));
		int randomCar = rand() % numCars;
		return this->cars.at(randomCar);
	}
	else {
		return this->cars.at(0);
	}

}

AssimpModel** ModelLibrary::getAnEmpty(){
	int numEmpties = this->curEmpties.size();

	if (numEmpties > 1)
		return &this->curEmpties.at(rand() % numEmpties);
	else
		return &this->curEmpties.at(0);
}

AssimpModel** ModelLibrary::getEmptyAt(int i){
	return &this->curEmpties.at(i);
}
