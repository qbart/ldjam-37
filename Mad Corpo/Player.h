#pragma once

#include "Node.h"
#include "SpriteNode.h"
#include <SFML/Graphics.hpp>
#include "Types.h"
#include "PositionableOnGrid.h"
#include "WalkableOnGrid.h"
#include "Events.h"
#include "Grid.h"


class Player : public Node, public PositionableOnGrid, public WalkableOnGrid
{
public:
	enum Direction { NONE, UP, DOWN, LEFT, RIGHT };
	enum State { STAY, WALK, SCAN, ROLL_CABLE };

public:
	Player();
	virtual ~Player();

protected:
	virtual void walkingDone();

private:
	virtual void updateCurrent(sf::Time dt);
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
	void onKeyReleased(Event* event);
	void onScanStop();

private:
	sf::CircleShape m_shape;
	bool m_serverPlugged = false;
	u8 m_action;
	Grid m_grid;
	Direction m_direction;
	State m_state;
};

