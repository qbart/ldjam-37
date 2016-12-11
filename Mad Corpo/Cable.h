#pragma once

#include "SpriteNode.h"
#include "Types.h"
#include "PositionableOnGrid.h"

class Computer;

class CablePart : public SpriteNode, public PositionableOnGrid
{
public:
	enum ID {
		HORIZONTAL = 0,
		VERTICAL = 1,
		BOTTOM_TO_RIGHT = 2,
		RIGHT_TO_BOTTOM = 2,
		TOP_TO_RIGHT = 3,
		RIGHT_TO_TOP = 3,
		TOP_TO_LEFT = 4,
		LEFT_TO_TOP = 4,
		BOTTOM_TO_LEFT = 5,
		LEFT_TO_BOTTOM = 5,
		TOP_TO_BOTTOM_SHORT = 6,
		LEFT_TO_RIGHT_SHORT = 7,
		BOTTOM_TO_TOP_SHORT = 8,
		RIGHT_TO_LEFT_SHORT = 9
	};

public:
	CablePart();
	virtual ~CablePart();
	void setType(ID type);

private:
	ID m_type;
	int m_variant;
};


class Cable : public Node
{
public:
	typedef std::vector<CablePart*> CablePartList;
public:
	Cable();
	virtual ~Cable();
	const int key() const { return m_key; }
	bool isBroken() const { return m_broken; }
	void disconnectAt(const GridPosition& position);
	void buildDefaultTopology(const GridPosition& connectorPosition);
	void put(const GridPosition& position);
	CablePartList getParts() const { return m_parts; }
	void setComputer(Computer* computer);

private:
	CablePart* buildPart(const GridPosition& at);

private:
	Computer *m_computer;
	CablePartList m_parts;
	bool m_broken;
	int m_key;
};

