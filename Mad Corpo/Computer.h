#pragma once

#include "SpriteNode.h"
#include <SFML/Graphics.hpp>
#include "PositionableOnGrid.h"

class Balloon;
class Person;

class Computer : public SpriteNode, public PositionableOnGrid
{
public:
	typedef enum { OFFLINE, ONLINE } Status;

public:
	Computer(u8 id);
	virtual ~Computer();
	void connect();
	void disconnect();
	Balloon* getBalloon() const;
	void setOwner(Person* owner);
	Person* getOwner() const { return m_owner; }
	bool isOffline() const { return m_status == OFFLINE; }

private:
	virtual void onAdd();

private:
	Status m_status;
	Person *m_owner;
	Balloon *m_statusBalloon;
	u8 m_id;
};
