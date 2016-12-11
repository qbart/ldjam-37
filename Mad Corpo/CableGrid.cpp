#include "CableGrid.h"
#include "Server.h"
#include "Cable.h"
#include "Computer.h"
#include "Person.h"
#include <cassert>


CableGrid::CableGrid(Server * server) :
	Node(),
	m_server(server),
	m_currentCable(nullptr)
{
	Events::get().observe("person.step", OnEvent(CableGrid::onPersonStep));
	Events::get().observe("server.cable.put", OnEvent(CableGrid::onCablePut));
}

CableGrid::~CableGrid()
{
}

Cable* CableGrid::createCable(const GridPosition& position, bool buildDefaultTopology)
{
	auto cable = new Cable();
	if (buildDefaultTopology)
	{
		cable->buildDefaultTopology(position);
	}
	m_cables.push_back(cable);
	addChild(cable);
	cacheCablePerTiles(cable);

	return cable;
}

void CableGrid::updateCurrent(sf::Time dt)
{
	while (!putQueue.empty())
	{
		auto cable = putQueue.back();

#ifdef DEBUG
		printf("Events: CableGrid#onCablePut at: (%d, %d), action: %d\n", cable.position.x, cable.position.y, cable.action);
#endif // DEBUG

		putQueue.pop_back();

		switch (cable.action)
		{
		case PLUG_IN:
#ifdef DEBUG
			printf("PLUG_IN: (%d, %d)\n", cable.position.x, cable.position.y);
#endif // DEBUG
			m_currentCable = new Cable();
			m_currentCable->put(cable.position);
			addChild(m_currentCable);
			break;

		case PLUG_ROLL:
#ifdef DEBUG
			printf("PLUG_ROLL: (%d, %d)\n", cable.position.x, cable.position.y);
#endif // DEBUG
			m_currentCable->put(cable.position);
			break;

		case PLUG_OUT:
#ifdef DEBUG
			printf("PLUG_OUT: (%d, %d)\n", cable.position.x, cable.position.y);
#endif // DEBUG
			removeChild(*m_currentCable);
			m_currentCable = nullptr;
			break;

		case PLUG_CONNECT:
#ifdef DEBUG
			printf("PLUG_CONNECT: (%d, %d)\n", cable.position.x, cable.position.y);
#endif // DEBUG
			auto computer = m_server->findComputerAt(cable.position);
			if (computer->isOffline())
			{
				cacheCablePerTiles(m_currentCable);
				m_currentCable->setComputer(computer);
				computer->connect();
			}
			else
			{
				removeChild(*m_currentCable);
			}
			m_currentCable = nullptr;
			break;
		}
	}

	while (!removalQueue.empty())
	{
		auto at = removalQueue.back();

		removalQueue.pop_back();

		if (m_cablesPerTile[at.key()].size() > 0)
		{
			auto cablePtr = std::find_if(
				m_cablesPerTile[at.key()].begin(),
				m_cablesPerTile[at.key()].end(), [&](Cable* cable) -> bool
			{
				return !cable->isBroken();
			});

			if (cablePtr != m_cablesPerTile[at.key()].end())
			{
				auto cable = (*cablePtr);
				cable->disconnectAt(at);

				for (auto gridPos : m_tilesPerCable[cable->key()])
				{
					auto found = std::find_if(
						m_cablesPerTile[gridPos.key()].begin(),
						m_cablesPerTile[gridPos.key()].end(), [&](Cable* ptr) -> bool
					{
						return ptr == cable;
					});
					assert(found != m_cablesPerTile[gridPos.key()].end());
					m_cablesPerTile[gridPos.key()].erase(found);
				}
				m_tilesPerCable[cable->key()].clear();

				auto found = std::find_if(m_cables.begin(), m_cables.end(), [&](Cable* ptr) -> bool
				{
					return ptr == cable;
				});
				if (found != m_cables.end())
					m_cables.erase(found);

				removeChild(*cable);
			}
		}
	}
}

void CableGrid::cacheCablePerTiles(Cable * cable)
{
	auto parts = cable->getParts();
	for (auto part : parts)
	{
		m_cablesPerTile[part->getGridPosition().key()].push_back(cable);
		m_tilesPerCable[cable->key()].push_back(part->getGridPosition());
#ifdef DEBUG
		printf("  CACHE: m_tilesPerCable %d -> (%d, %d)\n", cable->key(), part->getGridPosition().x, part->getGridPosition().y);
#endif // DEBUG
	}
}

void CableGrid::onPersonStep(Event * event)
{
	bool anyCablesHere = m_cablesPerTile[event->cable.position.key()].size() > 0;
	if (anyCablesHere)
	{
#ifdef DEBUG
		printf("Events: CableGrid#onPersonStep at: (%d, %d)\n", event->cable.position.x, event->cable.position.y);
#endif // DEBUG
		removalQueue.push_back(event->walk.position);
	}
}

void CableGrid::onCablePut(Event * event)
{
	putQueue.push_back(event->cable);
}
