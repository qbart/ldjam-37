#pragma once

#include <SFML/Graphics.hpp>
#include "Node.h"
#include "SpriteNode.h"

class Game
{
public:
	Game();
	virtual ~Game();

	int run();

private:
	void onCreate();
	void update(sf::Time dt);
	void render(sf::RenderWindow& window);
	void onDestroy();

	Node m_root;
};

