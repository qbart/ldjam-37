#pragma once

#include "SpriteNode.h"


class Balloon : public SpriteNode
{
public:
	Balloon();
	virtual ~Balloon();
	void setType(Balloons::ID id);
	void activate(sf::Time showIn, sf::Time hideAfter);
	bool isActivated() const { return m_activated; }

private:
	virtual void updateCurrent(sf::Time dt);

private:
	Balloons::ID m_type;
	bool m_activated;
	sf::Time m_showIn;
	sf::Time m_hideAfter;
};

