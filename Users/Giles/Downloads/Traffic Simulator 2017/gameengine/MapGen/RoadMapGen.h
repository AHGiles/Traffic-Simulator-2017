#pragma once

#include <time.h>
#include <vector>
#include <list>
#include <iostream>
using namespace std;

class RoadMapGen{
public:
	
	RoadMapGen();
	RoadMapGen(int sizeX, int sizeY);
	vector<vector<int>> genRoadMap();

private:
	void init();
	void getBaseMap();
	void closeDeadEnds();
	bool removeClusters();
	bool convertToRoadTypes();

	int maze_size_x;
	int maze_size_y;
	vector<vector<bool>> maze;
	vector<vector<int>> roadMap;
	list<pair<int, int>>drillers;
};