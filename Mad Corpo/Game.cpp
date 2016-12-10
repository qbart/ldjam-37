#include "Game.h"
#include <cassert>
#include <SFML/Graphics.hpp>
#include "Resources.h"
#include "Ids.h"
#include "Consts.h"

Game::Game()
{
}


Game::~Game()
{
}

int Game::run()
{

	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Mad Corpo", sf::Style::Titlebar | sf::Style::Close);

	onCreate();

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::KeyPressed:
				if (event.key.code == sf::Keyboard::Escape)
					window.close();
				break;
			}
			
		}

		render(window);
	}

	onDestroy();

	return 0;
}

void Game::onCreate()
{
	assert(Resources::get().load(Texture::ROOM, "assets/room.png"));

	auto room = new SpriteNode(Texture::ROOM);
	m_root.addChild(room);
	room->setPosition(sf::Vector2f(WIDTH / 2, HEIGHT / 2));
}

void Game::update(sf::Time dt)
{

}

void Game::render(sf::RenderWindow& window)
{
	window.clear(sf::Color(0x99, 0xcc, 0xff));
	window.draw(m_root);
	window.display();
}

void Game::onDestroy()
{
}