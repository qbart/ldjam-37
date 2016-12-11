#pragma once

#include "Node.h"
#include "PositionableOnGrid.h"
#include "WalkableOnGrid.h"


class Computer;

class Person : public Node, public PositionableOnGrid, public WalkableOnGrid
{
public:
	typedef enum { WORKING, LAZY, SUICIDAL, THIRSTY } Mood;
	typedef enum { SEAT, GRAB_A_COFFEE, GO_TO_PC, COMMIT_SUICIDE, FALLING, DEAD } State;
public:
	Person(Computer *computer);
	virtual ~Person();
	Mood getMood() const { return m_mood; }

protected:
	virtual void walkingDone();

private:
	virtual void updateCurrent(sf::Time dt);
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
	void become(Mood mood);

private:
	Mood m_mood;
	State m_state;
	int m_velocity;
	Computer *m_computer;
	sf::CircleShape m_shape;
	std::vector<GridPosition> m_walkingPath;
	GridPosition m_returnSpot;

	sf::Time m_counter;
	int m_cableStompsLeft;
	struct
	{
		sf::Time frequency;
		sf::Time timeToDecideToCommitSuicide;
		float coffeeAddict;
		float laziness;
		float clumsiness;
		int maxCableStomps;
	} m_personality;
};

