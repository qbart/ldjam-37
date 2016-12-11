#include "Balloon.h"



Balloon::Balloon() : 
	SpriteNode(Texture::ITEMS),
	m_activated(false)
{
	setOrigin(0, 24);
	setType(Balloons::WORKING);
	setVisible(false);
}

Balloon::~Balloon()
{
}

void Balloon::setType(Balloons::ID id)
{
	m_type = id;
	m_sprite.setTextureRect(sf::IntRect(32 * id, 0, 32, 32));
}

void Balloon::activate(sf::Time showIn, sf::Time hideAfter)
{
	m_showIn = showIn;
	m_hideAfter = hideAfter;
	m_activated = true;
}

void Balloon::updateCurrent(sf::Time dt)
{
	if (m_activated)
	{
		if (m_showIn.asSeconds() > 0)
		{
			m_showIn -= dt;
			if (m_showIn.asSeconds() <= 0)
				setVisible(true);
		}

		if (isVisible() && m_hideAfter.asSeconds() > 0)
		{
			m_hideAfter -= dt;
			if (m_hideAfter.asSeconds() <= 0)
			{
				m_activated = false;
				setVisible(false);
			}
		}
	}
}
