#include "Resources.h"
#include <cassert>


Resources::Resources()
{
}


Resources::~Resources()
{
}

sf::Texture& Resources::texture(Texture::ID id)
{
	auto found = m_textures.find(id);
	assert(found != m_textures.end());

	return *found->second;
}

bool Resources::load(Texture::ID id, const std::string& fpath)
{
	auto texture = new sf::Texture();
	bool result = texture->loadFromFile(fpath);
	TexturePtr ptr(texture);

	if (result)
	{
		m_textures.insert(
			std::make_pair(id, std::move(ptr))
		);
	}

	return result;
}
