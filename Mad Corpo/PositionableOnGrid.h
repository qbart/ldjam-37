#pragma once

#include "Types.h"
#include "Node.h"
#include "GridPosition.h"
#include <SFML/Graphics.hpp>

class PositionableOnGrid
{
public:
	PositionableOnGrid(sf::Transformable* transformable);
	virtual ~PositionableOnGrid();
	void setGridPosition(int x, int y);
	void setGridPosition(const GridPosition& position);
	GridPosition getGridPosition() const;
	sf::Transformable* getTransformable() { return m_transformable; }

protected:
	sf::Transformable *m_transformable = nullptr;
	GridPosition m_gridPosition;
	sf::Vector2f m_gridPositionOffset;
};

