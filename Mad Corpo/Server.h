#pragma once

#include "Node.h"
#include "Events.h"

class Computer;
class CableGrid;
class Cable;


class Server : public Node
{
public:
	Server();
	virtual ~Server();
	Computer* operator[](const int index) const;
	Computer* findComputerAt(const GridPosition& position) const;

private:
	virtual void onAdd();
	virtual void updateCurrent(sf::Time dt);
	void onScan();

private:
	CableGrid *m_cableGrid;
	Computer *m_computers[MAX_PEOPLE];
	bool m_scanning;
};


