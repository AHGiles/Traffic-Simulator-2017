#pragma once

#pragma once
#include "../ObjectLoading/Assimp/AssimpModel.h"
#include <vector>
#include <time.h>       /* time */

class ModelLibrary {

public:

	ModelLibrary();


	void swapModels();
	void swapEmpty();
	void swapStraight();
	void swapCorner();
	void swapTJunc();
	void swapCross();

	//void addCar(char* sFilePath);

	AssimpModel** getStraight();
	AssimpModel** getCorner();
	AssimpModel** getTJunc();
	AssimpModel** getCross();

	AssimpModel* getACar();
	AssimpModel** getAnEmpty();
	AssimpModel** getEmptyAt(int i);

private:

	bool twoD;

	//Road Models
	AssimpModel empty2D;
	std::vector<AssimpModel*> empties3D;
	AssimpModel straight2D, straight3D;
	AssimpModel corner2D, corner3D;
	AssimpModel tJunc2D, tJunc3D;
	AssimpModel cross2D, cross3D;

	AssimpModel *curStraight, *curCor, *curT, *curCross;

	//Car Models
	std::vector<AssimpModel*> cars;
	//Empty Models
	std::vector<AssimpModel*> curEmpties;

};

