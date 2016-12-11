#include "SpriteNode.h"
#include "Resources.h"

SpriteNode::SpriteNode(Texture::ID texId) :
	Node(),
	m_texId(texId)
{
	m_sprite.setTexture(getTexture());
	auto bounds = m_sprite.getLocalBounds();
	setOrigin(bounds.width / 2, bounds.height / 2);
}

SpriteNode::~SpriteNode()
{
}

sf::Texture & SpriteNode::getTexture()
{
	return Resources::get().texture(m_texId);
}

void SpriteNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_sprite, states);
}
