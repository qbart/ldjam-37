#include "GridPosition.h"


GridPosition::GridPosition(int x, int y)
{
	this->x = x;
	this->y = y;
}

GridPosition::~GridPosition()
{
}

GridPosition GridPosition::stepDown()
{
	GridPosition pos(x, y + 1);
	return pos;
}

GridPosition GridPosition::stepUp()
{
	GridPosition pos(x, y - 1);
	return pos;
}

GridPosition GridPosition::stepLeft()
{
	GridPosition pos(x - 1, y);
	return pos;
}

GridPosition GridPosition::stepRight()
{
	GridPosition pos(x + 1, y);
	return pos;
}
