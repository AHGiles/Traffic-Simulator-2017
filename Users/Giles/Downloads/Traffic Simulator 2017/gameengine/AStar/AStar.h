#pragma once

#include "AStarNode.h"
#include <glm/glm.hpp>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include <iostream>

#include "../GameObjects/Primitives/Direction.h"

enum roadType{
	E_CELL,		//0
	STR_WE,		//1
	STR_NS,		//2
	COR_NW,		//3
	COR_NE,		//4
	COR_SE,		//5
	COR_SW,		//6
	CRS_RD,		//7
	TJNC_NWE,	//8
	TJNC_ENS,	//9
	TJNC_SEW,	//10
	TJNC_WSN	//11
};

class AStar{
private: 
	std::vector<AStarNode*> openList;
	std::vector<AStarNode*> closedList;
	AStarNode* startNode;
	AStarNode* goalNode;
	std::vector<AStarNode*> mapNodes;
	int* map;
	std::vector<Direction>** carMap;
	int startDir;
	int gridSize;

public:
	AStar(int* map, bool d, int dir, int gridSize, std::vector<Direction>** carMap);
	int huristic(AStarNode* pos);
	std::vector<glm::vec2> search(glm::vec2 start, glm::vec2 end);
	AStarNode* getbestNode();
	std::vector<glm::vec2> getPath(AStarNode* end);
	void linkChild(AStarNode* parent, AStarNode temp, int weight, int dir);
	void updateParents(AStarNode* node);
	AStarNode* checkList(std::vector<AStarNode*> list, int id);
	int getBestNodePos(int id);
	void draw();
	bool debug;
	double timeCheckList;
	double updateParentsTime;
	int count;
	~AStar() {
		while (!mapNodes.empty()) {
			AStarNode* n = mapNodes.back();
			mapNodes.pop_back();
			delete n;
		}
	}
};