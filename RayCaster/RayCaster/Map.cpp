#include "Map.h"

Map::Map() {

}

Map::Map(int x, int y, std::vector<int> m, int size) {
	mapX = x;
	mapY = y;
	map = m;
	wallSize = size;
}

int Map::getMapX()
{
	return mapX;
}

int Map::getMapY()
{
	return mapY;
}

int Map::getWallSize() {
	return wallSize;
}

std::vector<int> Map::getMapMap()
{
	return map;
}
