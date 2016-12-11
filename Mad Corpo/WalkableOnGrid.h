#pragma once

#include "GridPosition.h"
#include "PositionableOnGrid.h"
#include <SFML/System/Time.hpp>

class WalkableOnGrid
{
public:
	WalkableOnGrid(PositionableOnGrid* positionable);
	virtual ~WalkableOnGrid();
	void walk(sf::Time dt, float velocity);

protected:
	virtual void walkingDone();

protected:
	PositionableOnGrid *m_positionable;
	GridPosition m_target;
	float m_stepX, m_stepY;
};

