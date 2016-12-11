#include "PositionableOnGrid.h"


PositionableOnGrid::PositionableOnGrid(sf::Transformable* transformable)
{
	m_transformable = transformable;
	m_gridPosition.x = 0;
	m_gridPosition.y = 0;
	m_gridPositionOffset.x = 1 * TILE_SIZE;
	m_gridPositionOffset.y = 2 * TILE_SIZE;
}

PositionableOnGrid::~PositionableOnGrid()
{
}

void PositionableOnGrid::setGridPosition(int x, int y)
{
	int gx = clamp(x, 0, TILES_COUNT - 1);
	int gy = clamp(y, 0, TILES_COUNT - 1);

	m_gridPosition.x = gx;
	m_gridPosition.y = gy;

	m_transformable->setPosition((gx * TILE_SIZE) + m_gridPositionOffset.x, (gy * TILE_SIZE) + m_gridPositionOffset.y);
}

void PositionableOnGrid::setGridPosition(const GridPosition & position)
{
	setGridPosition(position.x, position.y);
}

GridPosition PositionableOnGrid::getGridPosition() const
{
	return m_gridPosition;
}