#pragma once

#include "Node.h"
#include "Events.h"

class Cable;
class Server;

class CableGrid : public Node
{
public:
	typedef std::vector<Cable*> CableList;
	typedef std::vector<GridPosition> GridPositionList;

public:
	CableGrid(Server* server);
	virtual ~CableGrid();
	Cable* createCable(const GridPosition& position = GridPosition::ZERO, bool buildDefaultTopology = false);

private:
	virtual void updateCurrent(sf::Time dt);
	void cacheCablePerTiles(Cable* cable);
	void onPersonStep(Event* event);
	void onCablePut(Event* event);

private:
	Cable *m_currentCable;
	Server *m_server;
	CableList m_cables;
	std::vector<GridPosition> removalQueue;
	std::vector<CableEvent> putQueue;
	std::map<int, CableList> m_cablesPerTile;
	std::map<int, GridPositionList> m_tilesPerCable;
};


