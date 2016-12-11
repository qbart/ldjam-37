#include "Person.h"
#include "Computer.h"
#include "Balloon.h"
#include "Random.h"
#include "Events.h"


Person::Person(Computer *computer) :
	PositionableOnGrid(this),
	WalkableOnGrid(this),
	m_state(SEAT),
	m_mood(WORKING),
	m_shape(12.f)
{
	m_velocity = 8 * TILE_SIZE;
	m_computer = computer;
	computer->setOwner(this);
	setOrigin(sf::Vector2f(-5, -5));
	m_shape.setFillColor(sf::Color::Red);
	setGridPosition(computer->getGridPosition());

	// generate personality
	m_personality.frequency = sf::seconds(Random::range(2, 10));
	m_personality.timeToDecideToCommitSuicide = sf::seconds(Random::range(3, 10));
	m_personality.coffeeAddict = Random::percent();
	m_personality.laziness = Random::percent();
	m_personality.clumsiness = Random::percent();
	m_personality.maxCableStomps = 10;
	m_counter = m_personality.frequency;
	m_cableStompsLeft = m_personality.maxCableStomps;

#ifdef DEBUG
	printf("Person:\n");
	printf("  frequency = %0.2f\n", m_personality.frequency.asSeconds());
	printf("  timeToDecideToCommitSuicide = %0.2f\n", m_personality.timeToDecideToCommitSuicide.asSeconds());
	printf("  coffeeAddict = %0.2f\n", m_personality.coffeeAddict);
	printf("  laziness = %0.2f\n", m_personality.laziness);
	printf("  clumsiness = %0.2f\n", m_personality.clumsiness);
	printf("\n");
#endif // DEBUG
}

Person::~Person()
{
}

void Person::walkingDone()
{
	if (m_walkingPath.size() > 0)
	{
		auto oldTarget = m_target;
		m_target = m_walkingPath.back();
		m_walkingPath.pop_back();

		if (Random::percent() <= m_personality.clumsiness && m_cableStompsLeft > 0)
		{
			--m_cableStompsLeft;
			WalkEvent event;
			event.position = oldTarget;
			Event data;
			data.walk = event;
			Events::get().send("person.step", &data);
		}
	}
	else
	{
		if (m_state == GRAB_A_COFFEE)
		{
			Events::get().notify("coffee.drink");
			m_cableStompsLeft = m_personality.maxCableStomps;
			become(WORKING);

			m_state = GO_TO_PC;
			// make a return path
			m_walkingPath.push_back(m_target.stepLeft());

			// go to the left
			while (m_walkingPath.back().x >= 1)
				m_walkingPath.push_back(m_walkingPath.back().stepLeft());

			// go above your desk
			while (m_walkingPath.back().y >= m_returnSpot.y - 1)
				m_walkingPath.push_back(m_walkingPath.back().stepUp());

			while (m_walkingPath.back().x < m_returnSpot.x - 1)
				m_walkingPath.push_back(m_walkingPath.back().stepRight());

			// circle arund your desk
			m_walkingPath.push_back(m_walkingPath.back().stepRight());
			m_walkingPath.push_back(m_walkingPath.back().stepRight());
			if ((m_returnSpot.x >= 2 && m_returnSpot.x < 4) || (m_returnSpot.x >= 10 && m_returnSpot.x < 13))
				m_walkingPath.push_back(m_walkingPath.back().stepRight());
			m_walkingPath.push_back(m_walkingPath.back().stepDown());
			m_walkingPath.push_back(m_walkingPath.back().stepDown());

			while (m_walkingPath.back().x > m_returnSpot.x - 1)
				m_walkingPath.push_back(m_walkingPath.back().stepLeft());

			m_walkingPath.push_back(m_walkingPath.back().stepUp());

			std::reverse(m_walkingPath.begin(), m_walkingPath.end());
			walkingDone();
		}
		else if (m_state == GO_TO_PC)
		{
			m_state = SEAT;
		}
		else if (m_state == COMMIT_SUICIDE)
		{
			m_state = FALLING;
			auto pos = getPosition();
			setPosition(pos.x + TILE_SIZE * 2, pos.y);
			setOrigin(sf::Vector2f(0, 0));
			Events::get().notify("person.death");
		}
	}
}

void Person::updateCurrent(sf::Time dt)
{
	// handle states
	switch (m_state)
	{
	case DEAD:
		setVisible(false);
		setActive(false);
		break;

	case FALLING:
		if (getScale().x > 0)
			setScale(getScale().x - 0.05, getScale().y - 0.05);
		else
			m_state = DEAD;
		break;

	case GRAB_A_COFFEE:
	case GO_TO_PC:
	case COMMIT_SUICIDE:
		walk(dt, m_velocity);
		break;

	case SEAT:
		// handle moods
		if (m_mood == WORKING || m_mood == LAZY)
		{
			if (m_computer->isOffline())
			{
				become(SUICIDAL);
			}
		}

		if (m_mood == SUICIDAL)
		{
			if (!m_computer->isOffline())
			{
				become(WORKING);
			}
		}

		m_counter -= dt;
		if (m_counter.asSeconds() <= 0)
		{
			if (m_mood == SUICIDAL)
			{
				m_state = COMMIT_SUICIDE;
				m_walkingPath.clear();

				// leave your pc
				m_walkingPath.push_back(m_gridPosition.stepDown());

				// go to the right
				while (m_walkingPath.back().x < 15)
					m_walkingPath.push_back(m_walkingPath.back().stepRight());

				std::reverse(m_walkingPath.begin(), m_walkingPath.end());
				m_velocity *= 2;
				walkingDone(); // make first step
			}
			else
			{
				if (Random::percent() <= m_personality.coffeeAddict)
				{
					become(THIRSTY);
				}
				else if (Random::percent() <= m_personality.laziness)
				{
					become(LAZY);
				}

				m_counter = m_personality.frequency;
			}
		}
		break;
	}
}

void Person::drawCurrent(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(m_shape, states);
}

void Person::become(Mood mood)
{
	m_mood = mood;

	switch (mood)
	{
	case SUICIDAL:
		m_counter = m_personality.timeToDecideToCommitSuicide;
		break;

	case THIRSTY:
		m_state = GRAB_A_COFFEE;
		m_walkingPath.clear();

		// leave your pc
		m_walkingPath.push_back(m_gridPosition.stepDown());

		// go to the right
		while (m_walkingPath.back().x < 15)
			m_walkingPath.push_back(m_walkingPath.back().stepRight());

		// go to the bottom
		while (m_walkingPath.back().y < 14)
			m_walkingPath.push_back(m_walkingPath.back().stepDown());

		std::reverse(m_walkingPath.begin(), m_walkingPath.end());
		walkingDone(); // make first step
		m_returnSpot = m_walkingPath.back();
		break;
	}
}
