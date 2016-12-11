#pragma once

#include <SFML/Graphics.hpp>
#include "Node.h"
#include "SpriteNode.h"

class Player;
class Work;

class Game
{
public:
	Game();
	virtual ~Game();

	int run();

private:
	void onCreate();
	void processEvents(sf::RenderWindow& window);
	void update(sf::Time dt);
	void render(sf::RenderWindow& window);
	void onDestroy();

	Node m_root;
	Player *m_player;
	Work *m_work;
};

