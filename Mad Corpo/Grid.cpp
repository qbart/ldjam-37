#include "Grid.h"
#include "GridPosition.h"


Grid::Grid()
{
}

Grid::Grid(Data data)
{
	for (int y = 0; y < TILES_COUNT; ++y)
		for (int x = 0; x < TILES_COUNT; ++x)
			this->data[y][x] = data[y][x];
}

bool Grid::walkable(int x, int y)
{
	return isValidRange(x, y) && isWalkableTile(x, y);
}

bool Grid::actionable(int x, int y)
{
	return isValidRange(x, y) && isActionTile(x, y);
}

u8 Grid::at(const GridPosition & pos)
{
	if (isValidRange(pos.x, pos.y))
	{
		return data[pos.y][pos.x];
	}

	return TILE_INVALID;
}

bool Grid::isWalkableTile(int x, int y)
{
	return (data[y][x] == TILE_WALK || actionable(x, y));
}

bool Grid::isActionTile(int x, int y)
{
	return (data[y][x] == TILE_SEAT || data[y][x] == TILE_SERVER || data[y][x] == TILE_CONNECT);
}

bool Grid::isValidRange(int x, int y)
{
	return (x >= 0 && x < TILES_COUNT && y >= 0 && y < TILES_COUNT);
}
