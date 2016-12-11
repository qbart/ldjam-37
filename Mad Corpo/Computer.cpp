#include "Computer.h"
#include "Balloon.h"
#include "Ids.h"


Computer::Computer(u8 id) :
	SpriteNode(Texture::ITEMS),
	PositionableOnGrid(this),
	m_status(OFFLINE),
	m_owner(nullptr),
	m_id(id)
{
	setOrigin(0, 0);
	m_sprite.setTextureRect(sf::IntRect(3 * 32, 0, 32, 32));
}


Computer::~Computer()
{
}

void Computer::connect()
{
	m_status = ONLINE;
}

void Computer::disconnect()
{
	m_status = OFFLINE;
}

Balloon * Computer::getBalloon() const
{
	return m_statusBalloon;
}

void Computer::setOwner(Person* owner)
{
	m_owner = owner;
}

void Computer::onAdd()
{
	m_statusBalloon = new Balloon();
	addChild(m_statusBalloon);
}
