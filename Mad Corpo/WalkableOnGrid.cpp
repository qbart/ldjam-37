#include "WalkableOnGrid.h"
#include "Types.h"


WalkableOnGrid::WalkableOnGrid(PositionableOnGrid* positionable) :
	m_positionable(positionable)
{
	m_stepX = 0;
	m_stepY = 0;
}

WalkableOnGrid::~WalkableOnGrid()
{
}

void WalkableOnGrid::walk(sf::Time dt, float velocity)
{
	float distance = velocity * dt.asSeconds();
	float dx = distance * (m_target.x - m_positionable->getGridPosition().x);
	float dy = distance * (m_target.y - m_positionable->getGridPosition().y);

	m_stepX = clamp(m_stepX + dx, (float)-TILE_SIZE, (float)TILE_SIZE);
	m_stepY = clamp(m_stepY + dy, (float)-TILE_SIZE, (float)TILE_SIZE);

	m_positionable->setGridPosition(m_positionable->getGridPosition());
	auto position = m_positionable->getTransformable()->getPosition();
	position.x += m_stepX;
	position.y += m_stepY;
	m_positionable->getTransformable()->setPosition(position);

	if (fabs(m_stepX) >= (float)TILE_SIZE || fabs(m_stepY) >= (float)TILE_SIZE)
	{
		m_positionable->setGridPosition(m_target.x, m_target.y);

		m_stepX = 0;
		m_stepY = 0;
		walkingDone();
	}
}

void WalkableOnGrid::walkingDone()
{
}
