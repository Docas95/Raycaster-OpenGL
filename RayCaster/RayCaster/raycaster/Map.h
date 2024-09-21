#pragma once

#include <iostream>
#include <vector>

class Map
{
	private:
		int mapX;
		int mapY;
		int wallSize;
		std::vector<int> map;
	public:
		Map();
		Map(int x, int y, std::vector<int> m, int size);

		int getMapX();
		int getMapY();
		int getWallSize();
		std::vector<int> getMapMap();
};

