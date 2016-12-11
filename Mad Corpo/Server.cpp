#include "Server.h"
#include "Cable.h"
#include "Computer.h"
#include "Balloon.h"
#include "Person.h"
#include <cassert>
#include "CableGrid.h"


Server::Server() :
	Node(),
	m_scanning(false)
{
	Events::get().watch("system.scan.start", OnNotify(Server::onScan));
}

Server::~Server()
{
}

Computer* Server::operator[](const int index) const
{
	return m_computers[index];
}

Computer * Server::findComputerAt(const GridPosition & position) const
{
	for (int i = 0; i < MAX_PEOPLE; ++i)
	{
		if (m_computers[i]->getGridPosition() == position)
			return m_computers[i];
	}

	assert(false);

	return nullptr;
}

void Server::onAdd()
{
	m_cableGrid = new CableGrid(this);
	addChild(m_cableGrid);

	for (int i = 0; i < MAX_PEOPLE; ++i)
	{
		int y = (i / 8);
		int x = (i % 8);

		if (x >= 5)
			x += 2;

		if (x >= 3)
			x += 2;

		y *= 3;

		auto computer = new Computer(i + 1);
		m_computers[i] = computer;
		computer->setGridPosition(x + 2, y + 3);
		addChild(computer);

		auto cable = m_cableGrid->createCable(computer->getGridPosition(), true);
		cable->setComputer(m_computers[i]);
	}
}

void Server::updateCurrent(sf::Time dt)
{
	if (m_scanning)
	{
		bool allDeactivated = true;
		for (int i = 0; i < MAX_PEOPLE; ++i)
		{
			if (m_computers[i]->getBalloon()->isActivated())
			{
				allDeactivated = false;
				break;
			}
		}

		if (allDeactivated)
		{
			m_scanning = false;
			Events::get().notify("system.scan.stop");
		}
	}

	for (int i = 0; i < MAX_PEOPLE; ++i)
	{
		auto pc = m_computers[i];
		auto owner = pc->getOwner();
		auto balloon = pc->getBalloon();
		auto mood = owner->getMood();

		switch (mood)
		{
		case Person::THIRSTY:
		case Person::WORKING:
			balloon->setType(Balloons::WORKING);
			break;

		case Person::LAZY:
			balloon->setType(Balloons::NOT_WORKING);
			break;

		case Person::SUICIDAL:
			balloon->setType(Balloons::NO_CONNECTION);
			break;
		}
	}
}

void Server::onScan()
{
#ifdef DEBUG
	printf("Events: Server#onScan\n");
#endif // DEBUG

	m_scanning = true;

	int ms = 50;
	for (int i = 0; i < MAX_PEOPLE; ++i)
	{
		auto pc = m_computers[i];
		auto balloon = pc->getBalloon();
		balloon->activate(sf::milliseconds(ms), sf::seconds(2.5f));
		ms += 50;
	}
}
