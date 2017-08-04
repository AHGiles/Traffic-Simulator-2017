#include "AStar.h"
#include <time.h>
#include "../GameObjects/Roads/Road.h"

bool compareNodePointers(AStarNode* a, AStarNode* b){ return a->f > b->f; }

AStar::AStar(int* map, bool d, int dir, int gridSize, std::vector<Direction>** carMap){
	this->gridSize = gridSize;
	openList = std::vector<AStarNode*>();
	closedList = std::vector<AStarNode*>();
	mapNodes = std::vector<AStarNode*>(gridSize*gridSize);
	timeCheckList = 0;
	updateParentsTime = 0;
	count = 0;
	
	
	this->map = map;
	for(int y = 0; y < gridSize; y++){
		for(int x = 0; x < gridSize; x++){
			nodeStatus s = UNEXPLORED;
			int w = map[y*gridSize + x];
			if( w == -1)
				s = INVALID;
			mapNodes[y*gridSize + x] = new AStarNode(x,y,gridSize,s,w, -1);
		}
	}
	startNode = NULL;
	goalNode = NULL;
	debug = d;
	startDir = dir;
	this->carMap = carMap;
}
int AStar::huristic(AStarNode* pos){
	return 10 * (std::abs(goalNode->x - pos->x) + std::abs(goalNode->y - pos->y));
}
std::vector<glm::vec2> AStar::search(glm::vec2 start, glm::vec2 end){
	goalNode = mapNodes[(int)end.y * gridSize + end.x];
	startNode = mapNodes[(int)start.y * gridSize + start.x];
	startNode->g = 0;
	startNode->h = huristic(startNode);
	startNode->f = startNode->g+startNode->h;
	startNode->dir = startDir;
	//AStarNode* p = startNode;
	startNode->status = OPENLIST;
	openList.push_back(startNode);
	std::make_heap(openList.begin(), openList.end(),compareNodePointers);
	while(openList.size() > 0){
		AStarNode* b = getbestNode();
		//AStarNode* b = openList.at(0);
		//reached goal node, bail out
		if(b->id == goalNode->id)
			return getPath(b);

		AStarNode temp;
		//process b's children
		//Add valid children based on the current road piece type
		switch (map[b->id]) {

			case STR_WE:
				if (b->dir == WEST) {
				//west
					temp = AStarNode(b->x - 1, b->y,gridSize);
					if (temp.x >= 0 && map[temp.y*gridSize + temp.x] != 0)
						linkChild(b, temp, 10 + (carMap[temp.x][temp.y].size()*10), 3);
				}
				if (b->dir == EAST) {
					//east
					temp = AStarNode(b->x + 1, b->y, gridSize);
					if (temp.x < gridSize && map[temp.y*gridSize + temp.x] != 0)
						linkChild(b, temp, 10 + (carMap[temp.x][temp.y].size() * 10), 1);
				}
				break;
			case STR_NS:
				if (b->dir == SOUTH) {
					//south
					temp = AStarNode(b->x, b->y + 1, gridSize);
					if (temp.y < gridSize && map[temp.y*gridSize + temp.x] != 0)
						linkChild(b, temp, 10 + (carMap[temp.x][temp.y].size() * 10), 2);
				}
				if (b->dir == NORTH) {
					//north
					temp = AStarNode(b->x, b->y - 1, gridSize);
					if (temp.y >= 0 && map[temp.y*gridSize + temp.x] != 0)
						linkChild(b, temp, 10 + (carMap[temp.x][temp.y].size() * 10), 0);
				}
				break;
			case COR_NW:
				if (b->dir == EAST) {
					//north
					temp = AStarNode(b->x, b->y - 1, gridSize);
					if (temp.y >= 0 && map[temp.y*gridSize + temp.x] != 0)
						linkChild(b, temp, 10 + (carMap[temp.x][temp.y].size() * 10),0);
				}
				if (b->dir == SOUTH) {
					//west
					temp = AStarNode(b->x - 1, b->y, gridSize);
					if (temp.x >= 0 && map[temp.y*gridSize + temp.x] != 0)
						linkChild(b, temp, 10 + (carMap[temp.x][temp.y].size() * 10), 3);
				}
				break;
			case COR_NE:
				if (b->dir == WEST) {
					//north
					temp = AStarNode(b->x, b->y - 1, gridSize);
					if (temp.y >= 0 && map[temp.y*gridSize + temp.x] != 0)
						linkChild(b, temp, 10 + (carMap[temp.x][temp.y].size() * 10), 0);
				}
				if (b->dir == SOUTH) {
					//east
					temp = AStarNode(b->x + 1, b->y, gridSize);
					if (temp.x < gridSize && map[temp.y*gridSize + temp.x] != 0)
						linkChild(b, temp, 10 + (carMap[temp.x][temp.y].size() * 10), 1);
				}
				break;
			case COR_SE:
				if (b->dir == WEST) {
					//south
					temp = AStarNode(b->x, b->y + 1, gridSize);
					if (temp.y < gridSize && map[temp.y*gridSize + temp.x] != 0)
						linkChild(b, temp, 10 + (carMap[temp.x][temp.y].size() * 10), 2);
				}
				if (b->dir == NORTH) {
					//east
					temp = AStarNode(b->x + 1, b->y, gridSize);
					if (temp.x < gridSize && map[temp.y*gridSize + temp.x] != 0)
						linkChild(b, temp, 10 + (carMap[temp.x][temp.y].size() * 10), 1);
				}
				break;
			case COR_SW:
				if (b->dir == EAST) {
					//south
					temp = AStarNode(b->x, b->y + 1, gridSize);
					if (temp.y < gridSize && map[temp.y*gridSize + temp.x] != 0)
						linkChild(b, temp, 10 + (carMap[temp.x][temp.y].size() * 10), 2);
				}
				if (b->dir == NORTH) {
					//west
					temp = AStarNode(b->x - 1, b->y, gridSize);
					if (temp.x >= 0 && map[temp.y*gridSize + temp.x] != -1)
						linkChild(b, temp, 10 + (carMap[temp.x][temp.y].size() * 10), 3);
				}
				break;
			case CRS_RD:
				if (b->dir != SOUTH) {
					//south
					temp = AStarNode(b->x, b->y + 1, gridSize);
					if (temp.y < gridSize && map[temp.y*gridSize + temp.x] != 0)
						linkChild(b, temp, 20 + (carMap[temp.x][temp.y].size() * 10), 2);
				}
				if (b->dir != WEST) {
					//west
					temp = AStarNode(b->x - 1, b->y, gridSize);
					if (temp.x >= 0 && map[temp.y*gridSize + temp.x] != 0)
						linkChild(b, temp, 20 + (carMap[temp.x][temp.y].size() * 10), 3);
				}
				if (b->dir != NORTH) {
					//north
					temp = AStarNode(b->x, b->y - 1, gridSize);
					if (temp.y >= 0 && map[temp.y*gridSize + temp.x] != 0)
						linkChild(b, temp, 20 + (carMap[temp.x][temp.y].size() * 10), 0);
				}
				if (b->dir != EAST) {
					//east
					temp = AStarNode(b->x + 1, b->y, gridSize);
					if (temp.x < gridSize && map[temp.y*gridSize + temp.x] != 0)
						linkChild(b, temp, 20 + (carMap[temp.x][temp.y].size() * 10), 1);
				}
				break;
			case TJNC_NWE:
				if (b->dir == SOUTH || b->dir == WEST) {
					//west
					temp = AStarNode(b->x - 1, b->y, gridSize);
					if (temp.x >= 0 && map[temp.y*gridSize + temp.x] != 0)
						linkChild(b, temp, 20 + (carMap[temp.x][temp.y].size() * 10), 3);
				}
				if (b->dir == EAST || b->dir == WEST) {
					//north
					temp = AStarNode(b->x, b->y - 1, gridSize);
					if (temp.y >= 0 && map[temp.y*gridSize + temp.x] != 0)
						linkChild(b, temp, 20 + (carMap[temp.x][temp.y].size() * 10), 0);
				}
				if (b->dir == SOUTH || b->dir == EAST) {
					//east
					temp = AStarNode(b->x + 1, b->y, gridSize);
					if (temp.x < gridSize && map[temp.y*gridSize + temp.x] != 0)
						linkChild(b, temp, 20 + (carMap[temp.x][temp.y].size() * 10), 1);
				}
				break;
			case TJNC_ENS:
				if (b->dir == NORTH || b->dir == WEST) {
					//north
					temp = AStarNode(b->x, b->y - 1, gridSize);
					if (temp.y >= 0 && map[temp.y*gridSize + temp.x] != 0)
						linkChild(b, temp, 20 + (carMap[temp.x][temp.y].size() * 10), 0);
				}
				if (b->dir == SOUTH || b->dir == NORTH) {
					//east
					temp = AStarNode(b->x + 1, b->y, gridSize);
					if (temp.x < gridSize && map[temp.y*gridSize + temp.x] != 0)
						linkChild(b, temp, 20 + (carMap[temp.x][temp.y].size() * 10), 1);
				}
				if (b->dir == SOUTH || b->dir == WEST) {
					//south
					temp = AStarNode(b->x, b->y + 1, gridSize);
					if (temp.y < gridSize && map[temp.y*gridSize + temp.x] != 0)
						linkChild(b, temp, 20 + (carMap[temp.x][temp.y].size() * 10), 2);
				}
				break;
			case TJNC_SEW:
				if (b->dir == EAST || b->dir == WEST) {
					//south
					temp = AStarNode(b->x, b->y + 1, gridSize);
					if (temp.y < gridSize && map[temp.y*gridSize + temp.x] != 0)
						linkChild(b, temp, 20 + (carMap[temp.x][temp.y].size() * 10), 2);
				}
				if (b->dir == NORTH || b->dir == EAST) {
					//east
					temp = AStarNode(b->x + 1, b->y, gridSize);
					if (temp.x < gridSize && map[temp.y*gridSize + temp.x] != 0)
						linkChild(b, temp, 20 + (carMap[temp.x][temp.y].size() * 10), 1);
				}
				if (b->dir == NORTH || b->dir == WEST) {
					//west
					temp = AStarNode(b->x - 1, b->y, gridSize);
					if (temp.x >= 0 && map[temp.y*gridSize + temp.x] != 0)
						linkChild(b, temp, 20 + (carMap[temp.x][temp.y].size() * 10), 3);
				}
				break;
			case TJNC_WSN:
				if (b->dir == SOUTH || b->dir == NORTH) {
					//west
					temp = AStarNode(b->x - 1, b->y, gridSize);
					if (temp.x >= 0 && map[temp.y*gridSize + temp.x] != 0)
						linkChild(b, temp, 20 + (carMap[temp.x][temp.y].size() * 10), 3);
				}
				if (b->dir == SOUTH || b->dir == EAST) {
					//south
					temp = AStarNode(b->x, b->y + 1, gridSize);
					if (temp.y < gridSize && map[temp.y*gridSize + temp.x] != 0)
						linkChild(b, temp, 20 + (carMap[temp.x][temp.y].size() * 10), 2);
				}
				if (b->dir == NORTH || b->dir == EAST) {
					//north
					temp = AStarNode(b->x, b->y - 1, gridSize);
					if (temp.y >= 0 && map[temp.y*gridSize + temp.x] != 0)
						linkChild(b, temp, 20 + (carMap[temp.x][temp.y].size() * 10), 1);
				}
				break;

		}
		/*//left
		temp = AStarNode(b->x-1,b->y);
		if(temp.x >= 0 && map[temp.y*gridSize + temp.x] != -1)
			linkChild(b, temp, map[temp.id] * 10);
		//bot
		temp = AStarNode(b->x,b->y+1);
		if(temp.y < gridSize && map[temp.y*gridSize + temp.x] != -1)
			linkChild(b, temp, map[temp.id] * 10);
		//right
		temp = AStarNode(b->x+1,b->y);
		if(temp.x < gridSize && map[temp.y*gridSize + temp.x] != -1)
			linkChild(b, temp, map[temp.id] * 10);
		//top
		temp = AStarNode(b->x,b->y-1);
		if(temp.y >= 0 && map[temp.y*gridSize + temp.x] != -1)
			linkChild(b, temp, map[temp.id] * 10);
		*/
		closedList.push_back(b);
		b->status = CLOSEDLIST;
		//std::pop_heap(openList.begin(), openList.end(),compareNodePointers);
		//openList.pop_back();
		openList.erase(openList.begin());
		//std::make_heap(openList.begin(), openList.end(),compareNodePointers);
		if(debug){
			draw();
			system("pause");
		}
	}
	//no path found
	std::vector<glm::vec2> path = std::vector<glm::vec2>();
	if (closedList.size() > 1)
		path.push_back(glm::vec2(closedList[1]->x,closedList[1]->y));
	path.push_back(start);
	return path;
}
AStarNode* AStar::getbestNode(){
	std::make_heap(openList.begin(), openList.end(),compareNodePointers);
	//std::sort(openList.begin(), openList.end());

	/*AStarNode* best = openList.at(0);
	for each(AStarNode* n in openList){
		if(n->f < best->f)
			best = n;
	}
	return best;*/
	return openList[0];
}

int AStar::getBestNodePos(int id){
	for(int i = 0; i < openList.size(); i++){
		if(openList.at(i)->id == id)
			return i;
	}
	return 0;
}

std::vector<glm::vec2> AStar::getPath(AStarNode* end){
	std::vector<glm::vec2> path = std::vector<glm::vec2>();
	path.push_back(glm::vec2(end->x,end->y));
	AStarNode* current = end->parent;
	while(current->id != startNode->id){
		path.push_back(glm::vec2(current->x, current->y));
		current = current->parent;
	}
	return path;
}

void AStar::linkChild(AStarNode* parent, AStarNode temp, int weight, int d){
	count++;
	clock_t s = clock();
	int x = temp.x;
	int y = temp.y;
	int g = parent->g + weight;
	int id = temp.id;

	AStarNode* check = NULL;

	if(mapNodes[id]->status == OPENLIST){
		check = mapNodes[id];
		parent->children[parent->numChildren++] = check;

		if(g < check->g){
			check->parent = parent;
			check->g = g;
			check->f = g + check->h;
			check->dir = d;
		}
	}
	else if(mapNodes[id]->status == CLOSEDLIST){
		check = mapNodes[id];
		parent->children[parent->numChildren++] = check;

		if(g < check->g){
			check->parent = parent;
			check->g = g;
			check->f = g + check->h;
			check->dir = d;

			updateParents(check);
		}
	}
	else{
		AStarNode* node = mapNodes[id];
		node->parent = parent;
		node->g = g;
		node->h = huristic(node);
		node->f = node->g + node->h;
		node->status = OPENLIST;
		node->dir = d;
		
		//if(!std::is_heap(openList.begin(), openList.end(),compareNodePointers))
			//std::make_heap(openList.begin(), openList.end(),compareNodePointers);
		openList.push_back(node);
		//std::push_heap(openList.begin(), openList.end(),compareNodePointers);

		parent->children[parent->numChildren++] = node;
	}
	clock_t e = clock() - s;
	updateParentsTime += ((double)e / ((double)CLOCKS_PER_SEC));
}

void AStar::updateParents(AStarNode* node){
	clock_t s = clock();
	int g = node->g;
	int c = node->numChildren;

	AStarNode* child = NULL;
	std::vector<AStarNode*> stack = std::vector<AStarNode*>();
	for(int i = 0; i < c; i++){
		child = node->children[i];
		if(g+1 < child->g){
			child->g = g+1;
			child->f = child->g + child->h;
			child->parent = node;

			stack.push_back(child);
		}
	}

	AStarNode* parent;
	while(stack.size() > 0){
		parent = stack.back();
		stack.pop_back();
		c = parent->numChildren;

		for(int i = 0; i < c; i++){
			child = parent->children[i];
			
			if(parent->g+1 < child->g){
				int mapVal = child->g - child->parent->g;
				int w = 10;
				int newW = 10;

				mapVal /= w;
				child->g = parent->g + w * mapVal;
				child->f = child->g + child->h;
				child->parent = parent;

				stack.push_back(child);
			}
		}
	}
	clock_t e = clock() - s;
	updateParentsTime += ((double)e / ((double)CLOCKS_PER_SEC));
}

AStarNode* AStar::checkList(std::vector<AStarNode*> list, int id){
	clock_t s = clock();
	for each(AStarNode* n in list){
		if(n->id == id){
			clock_t e = clock() - s;
			timeCheckList += ((double)e / ((double)CLOCKS_PER_SEC));
			return n;
		}
	}
	clock_t e = clock() - s;
	timeCheckList += ((double)e / ((double)CLOCKS_PER_SEC));
	return NULL;
}

void AStar::draw(){
	for(int y = 0; y < gridSize; y++){
		std::cout << std::endl;
		for(int x = 0; x < gridSize; x++){
			bool inPath = false;
			if(x == startNode->x && y == startNode->y)
				std::cout << "S";
			else
				if(x == goalNode->x && y == goalNode->y)
					std::cout << "E";
				else{
					if(mapNodes[y * gridSize + x]->status == OPENLIST)
						std::cout << "O";
					if(mapNodes[y * gridSize + x]->status == CLOSEDLIST)
						std::cout << "C";
					if(mapNodes[y * gridSize + x]->status == INVALID)
						std::cout << "0";
					if(mapNodes[y * gridSize + x]->status == UNEXPLORED)
						std::cout << "1";
				}

		}
	}
	std::cout << "\n\n" << std::endl;
}