#include "Game.h"
#include <cassert>
#include <SFML/Graphics.hpp>
#include "Resources.h"
#include "Ids.h"
#include "Player.h"
#include "Work.h"
#include "Events.h"
#include "Random.h"
#include "Sfx.h"


const sf::Time TIME_PER_FRAME = sf::seconds(1 / 60.f);

Game::Game()
{
}


Game::~Game()
{
}

int Game::run()
{
	Random::init();
	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Mad Corpo", sf::Style::Titlebar | sf::Style::Close);
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;

	onCreate();

	Sfx::get().music(true);

	while (window.isOpen())
	{
		processEvents(window);

		timeSinceLastUpdate += clock.restart();
		while (timeSinceLastUpdate > TIME_PER_FRAME)
		{
			timeSinceLastUpdate -= TIME_PER_FRAME;
			processEvents(window);
			update(TIME_PER_FRAME);
		}

		render(window);
	}

	onDestroy();

	return 0;
}

void Game::onCreate()
{
	Sfx::get();
	assert(Resources::get().load(Texture::ROOM, "assets/room.png"));
	assert(Resources::get().load(Texture::ITEMS, "assets/items.png"));
	assert(Resources::get().load(Texture::CABLE, "assets/cable.png"));

	auto room = new SpriteNode(Texture::ROOM);
	room->setPosition(WIDTH / 2, HEIGHT / 2);

	m_work = new Work();

	m_player = new Player();
	m_player->setGridPosition(7, 1);

	room->addChild(m_work);
	room->addChild(m_player);

	m_root.addChild(room);
}

void Game::processEvents(sf::RenderWindow& window)
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
		case sf::Event::KeyReleased:
			Event data;
			data.key = event.key;
			Events::get().send("input.keyReleased", &data);
			break;
		}
	}
}

void Game::update(sf::Time dt)
{
	m_root.update(dt);
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