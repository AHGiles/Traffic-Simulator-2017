#include "RoadMapGen.h"

RoadMapGen::RoadMapGen()
{
	srand(time(NULL));

	maze_size_x = 35;
	maze_size_y = 15;

	maze.resize(maze_size_x); // Resize our vector to the specified coords
	roadMap.resize(maze_size_x);
	for (size_t x = 0; x < maze_size_x; x++) {
		maze[x].resize(maze_size_y);
		roadMap[x].resize(maze_size_y);
	}

}

RoadMapGen::RoadMapGen(int sizeX, int sizeY)
{
	srand(time(NULL));

	maze_size_x = sizeX;
	maze_size_y = sizeY;

	maze.resize(maze_size_x); // Resize our vector to the specified coords
	roadMap.resize(maze_size_x);
	for (size_t x = 0; x < maze_size_x; x++) {
		maze[x].resize(maze_size_y);
		roadMap[x].resize(maze_size_y);
	}
}

void RoadMapGen::init() {

	srand(time(NULL));

	for (size_t x = 0; x < maze_size_x; x++) {
		for (size_t y = 0; y < maze_size_y; y++) {
			maze[x][y] = false; // Set all the values in the maze to false
			roadMap[x][y] = -1;
		}
	}
}

vector<vector<int>> RoadMapGen::genRoadMap()
{
	bool validMap = false;
	while (!validMap) {
		init();
		getBaseMap();
		closeDeadEnds();
		bool repeat = removeClusters();
		while (repeat) {
			closeDeadEnds();
			repeat = removeClusters();
		}
		validMap = convertToRoadTypes();

	}

	/*for (size_t y = 0; y < maze_size_y; y++)
	{
		for (size_t x = 0; x < maze_size_x; x++)
		{
			if (maze[x][y] == true)
				printf(".");
			else
				printf("#");
		}
		printf("\n");
	}
	printf("\n");*/

	/*for (size_t y = 0; y < maze_size_y; y++)
	{
		for (size_t x = 0; x < maze_size_x; x++)
		{
			if (roadMap[y][x] < 10)
				printf("0%d ",roadMap[y][x]);
			else
				printf("%d ", roadMap[y][x]);
		}
		printf("\n");
	}*/

	return roadMap;
}

void RoadMapGen::getBaseMap()
{
	drillers.push_back(make_pair(maze_size_x / 2, maze_size_y / 2));

	while (drillers.size()>0)
	{
		list < pair < int, int> >::iterator m, _m, temp;
		m = drillers.begin();
		_m = drillers.end();
		while (m != _m)
		{
			bool remove_driller = false;
			switch (rand() % 4)
			{
			case 0:
				(*m).second -= 2;
				if ((*m).second<0 || maze[(*m).first][(*m).second])
				{
					remove_driller = true;
					break;
				}
				maze[(*m).first][(*m).second + 1] = true;
				break;
			case 1:
				(*m).second += 2;
				if ((*m).second >= maze_size_y || maze[(*m).first][(*m).second])
				{
					remove_driller = true;
					break;
				}
				maze[(*m).first][(*m).second - 1] = true;
				break;
			case 2:
				(*m).first -= 2;
				if ((*m).first<0 || maze[(*m).first][(*m).second])
				{
					remove_driller = true;
					break;
				}
				maze[(*m).first + 1][(*m).second] = true;
				break;
			case 3:
				(*m).first += 2;
				if ((*m).first >= maze_size_x || maze[(*m).first][(*m).second])
				{
					remove_driller = true;
					break;
				}
				maze[(*m).first - 1][(*m).second] = true;
				break;
			}
			if (remove_driller)
				m = drillers.erase(m);
			else
			{
				drillers.push_back(make_pair((*m).first, (*m).second));
				// uncomment the line below to make the maze easier 
				//if (rand()%2 && maze_size_x*maze_size_y < 25*25) 
				drillers.push_back(make_pair((*m).first, (*m).second));

				maze[(*m).first][(*m).second] = true;
				++m;
			}

			/*for (size_t y = 0; y < maze_size_y; y++)
			{
				for (size_t x = 0; x < maze_size_x; x++)
				{
					if (maze[y][x] == true)
						printf(".");
					else
						printf("#");
				}
				printf("\n");
				
			}
			system("pause");*/
		}
	}

	//// Print out map 
	//for (size_t y = 0; y < maze_size_y; y++)
	//{
	//	for (size_t x = 0; x < maze_size_x; x++)
	//	{
	//		if (maze[y][x] == true)
	//			printf(".");
	//		else
	//			printf("#");
	//	}
	//	printf("\n");
	//}

	//cout << " ------------------ " << endl;
}

void RoadMapGen::closeDeadEnds()
{
	// Deadend removal

	bool changes = true;
	while (changes)
	{

		changes = false;
		for (int x = 0; x < maze_size_x; x++)
		{
			for (int y = 0; y < maze_size_y; y++)
			{
				int count = 0;
				int loc = 0;
				if (maze[x][y]) // If it's a road piece
				{
					//cout << y << endl;
					if ((y - 1) >= 0)
					{ // Check up for road piece
						if (maze[x][y - 1]) {
							count++;
							loc = 1;
						}
					}
					if ((y + 1) < maze_size_y)
					{ // Check down for road piece
						if (maze[x][y + 1]) {
							count++;
							loc = 2;
						}
					}
					if ((x - 1) >= 0)
					{ // Check left for road piece
						if (maze[x-1][y]) {
							count++;
							loc = 3;
						}
					}
					if ((x + 1) < maze_size_x)
					{ // Check right for road piece
						if (maze[x + 1][y]) {
							count++;
							loc = 4;
						}
					}
					if (count == 1) // We have a deadend if only one exit/entrance
					{
						//cout << x << ", " << y << endl;
						bool pathFound = false;
						int next;
						switch (loc) {
						case 1:
							next = y + 1;
							while (next < maze_size_y - 1 && !pathFound) {
								next++;
								if (maze[x][next]) {
									pathFound = true;
								}
							}
							if (pathFound) {
								for (int i = y + 1; i < next; i++) {
									maze[x][i] = true;
								}
							}
							break;
						case 2:
							next = y - 1;
							while (next > 0 && !pathFound) {
								next--;
								if (maze[x][next]) {
									pathFound = true;
								}
							}
							if (pathFound) {
								for (int i = y - 1; i > next; i--) {
									maze[x][i] = true;
								}
							}
							break;
						case 3:
							next = x + 1;
							while (next < maze_size_x - 1 && !pathFound) {
								next++;
								if (maze[next][y]) {
									pathFound = true;
								}
							}
							if (pathFound) {
								for (int i = x + 1; i < next; i++) {
									maze[i][y] = true;
								}
							}
							break;
						case 4:
							next = x - 1;
							while (next > 0 && !pathFound) {
								next--;
								if (maze[next][y]) {
									pathFound = true;
								}
							}
							if (pathFound) {
								for (int i = x - 1; i > next; i--) {
									maze[i][y] = true;
								}
							}
							break;
						}

						if (!pathFound) {

							switch (rand() % 4)
							{
							case 0:
								if ((y - 1) > 0) // Add checks for further into the direction
									maze[x][y - 1] = true;
								break;
							case 1:
								if ((y + 1) < maze_size_y - 1)
									maze[x][y + 1] = true;
								break;
							case 2:
								if ((x - 1) > 0)
									maze[x - 1][y] = true;
								break;
							case 3:
								if ((x + 1) < maze_size_x - 1)
									maze[x + 1][y] = true;
								break;
							}

						}
						changes = true;
					}
				}

			}

		}

		/*for (size_t y = 0; y < maze_size_y; y++)
		{
			for (size_t x = 0; x < maze_size_x; x++)
			{
				if (maze[y][x] == true)
					printf(".");
				else
					printf("#");
			}
			printf("\n");
		}

		cout << " ------------------ " << endl;
		system("pause");*/
	}
}

bool RoadMapGen::removeClusters()
{
	bool changeMade = false;
	for (int y = 0; y < maze_size_y - 1; y++) {
		for (int x = 0; x < maze_size_x - 1; x++) {
			//pass 4x4 window over grid, if all 4 are true process cells by extending search area
			if (maze[x][y] && maze[x][y + 1] && maze[x + 1][y] && maze[x + 1][y + 1]) {
				maze[x][y] = false; 
				maze[x][y + 1] = false; 
				maze[x + 1][y] = false; 
				maze[x + 1][y + 1] = false;
				changeMade = true;
				/*if (!maze[x][y - 1] && !maze[x - 1][y]) {
					maze[x][y] = false;
					changeMade = true;
				}
				if (!maze[x][y + 2] && !maze[x - 1][y + 1]) {
					maze[x][y + 1] = false;
					changeMade = true;
				}
				if (!maze[x + 1][y - 1] && !maze[x + 2][y]) {
					maze[x + 1][y] = false;
					changeMade = true;
				}
				if (!maze[x + 1][y + 2] && !maze[x + 2][y + 1]) {
					maze[x + 1][y + 1] = false;
					changeMade = true;
				}*/
			}
		}
	}
	return changeMade;
}

bool RoadMapGen::convertToRoadTypes()
{
	for (int y = 0; y < maze_size_y; y++) {
		for (int x = 0; x < maze_size_x; x++) {
			if (maze[x][y]) {
				int count = 0;
				bool loc[4] = { false, false, false, false };
				if ((y - 1) >= 0)
				{ // Check up for road piece
					if (maze[x][y - 1]) {
						count++;
						loc[0] = true;
					}
				}
				if ((y + 1) < maze_size_y)
				{ // Check down for road piece
					if (maze[x][y + 1]) {
						count++;
						loc[1] = true;
					}
				}
				if ((x - 1) >= 0)
				{ // Check left for road piece
					if (maze[x - 1][y]) {
						count++;
						loc[2] = true;
					}
				}
				if ((x + 1) < maze_size_x)
				{ // Check right for road piece
					if (maze[x + 1][y]) {
						count++;
						loc[3] = true;
					}
				}
				/*
					EMPTY,		//0		
					STR_WE,		//1		--
					STR_NS,		//2		|
					COR_NW,		//3		_|
					COR_NE,		//4		|_
					COR_SE,		//5		|"
					COR_SW,		//6		"|
					CRS_RD,		//7		+
					TJNC_NWE,	//8		_|_
					TJNC_ENS,	//9		|-
					TJNC_SEW,	//10	"|"
					TJNC_WSN	//11	-|			
				*/
				switch (count) {
				case 0: case 1://invalid singular/deadend
					//cout << "dead end present" << endl;
					roadMap[0][0] = -1;
					return false;
				case 2:
					//str or cor
					if (loc[2] && loc[3])
						roadMap[x][y] = 1;
					if (loc[0] && loc[1])
						roadMap[x][y] = 2;
					if (loc[0] && loc[2])
						roadMap[x][y] = 3;
					if (loc[0] && loc[3])
						roadMap[x][y] = 4;
					if (loc[1] && loc[3])
						roadMap[x][y] = 5;
					if (loc[1] && loc[2])
						roadMap[x][y] = 6;
					break;
				case 3:
					if (loc[0] && loc[2] && loc[3])
						roadMap[x][y] = 8;
					if (loc[0] && loc[3] && loc[1])
						roadMap[x][y] = 9;
					if (loc[1] && loc[3] && loc[2])
						roadMap[x][y] = 10;
					if (loc[1] && loc[0] && loc[2])
						roadMap[x][y] = 11;
					//TJunc
					break;
				case 4:
					roadMap[x][y] = 7;
					//crossroad
					break;
				}

			}
			else {
				roadMap[x][y] = 0;
			}
		}
	}

	for (int y = 0; y < maze_size_y; y++) {
		for (int x = 0; x < maze_size_x; x++) {
			if (roadMap[x][y] > 6) {
				if ((y - 1) >= 0)
				{ // Check up for road piece
					if (roadMap[x][y - 1] > 6) {
						//cout << "invalid piece arrangement" << endl;
						return false;
					}
				}
				if ((y + 1) < maze_size_y)
				{ // Check down for road piece
					if (roadMap[x][y + 1] > 6) {
						//cout << "invalid piece arrangement" << endl;
						return false;
					}
				}
				if ((x - 1) >= 0)
				{ // Check left for road piece
					if (roadMap[x - 1][y] > 6) {
						//cout << "invalid piece arrangement" << endl;
						return false;
					}
				}
				if ((x + 1) < maze_size_x)
				{ // Check right for road piece
					if (roadMap[x + 1][y] > 6) {
						//cout << "invalid piece arrangement" << endl;
						return false;
					}
				}
			}
		}
	}

	return true;
}
