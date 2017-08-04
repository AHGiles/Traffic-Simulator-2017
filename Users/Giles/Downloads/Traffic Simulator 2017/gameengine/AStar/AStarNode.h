#pragma once
const int MAP_WIDTH = 25;
const int MAP_HEIGHT = 25;

enum nodeStatus{
	UNEXPLORED, 
	OPENLIST,
	CLOSEDLIST,
	INVALID,
	VALID
};

class AStarNode{
public:
	AStarNode(int x = 0, int y = 0, int gridSize = 0, nodeStatus s = UNEXPLORED, int w = 1, int d = -1){
		this->x = x;
		this->y = y;
		id = y * gridSize + x;
		numChildren = 0;
		f = g = h = 0;
		status = s;
		weight = w;
		dir = d;
	}

	bool operator<(const AStarNode* node)const{ return this->f < node->f;}

	int f, g, h;
	int x, y;
	int numChildren;
	int id;
	int weight;
	int dir;
	AStarNode* parent;
	AStarNode* next;
	AStarNode* children[8];
	nodeStatus status;
};

