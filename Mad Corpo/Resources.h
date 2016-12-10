#pragma once

#include "Ids.h"
#include <SFML/Graphics.hpp>
#include <memory>

class Resources
{
public:
	typedef std::unique_ptr<sf::Texture> TexturePtr;

public:
	static Resources& get()
	{
		static Resources instance;
		return instance;
	}
	~Resources();

	sf::Texture& texture(Texture::ID id);
	bool load(Texture::ID id, const std::string& fpath);

private:
	Resources();

private:
	std::map<Texture::ID, TexturePtr> m_textures;
};

