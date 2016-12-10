#pragma once

#include "Node.h"
#include <SFML/Graphics.hpp>
#include "Ids.h"


class SpriteNode : public Node
{
public:
	SpriteNode(Texture::ID texId);
	virtual ~SpriteNode();
	sf::Texture& getTexture();

private:
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	Texture::ID m_texId;
	sf::Sprite m_sprite;
};

