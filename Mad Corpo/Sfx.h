#pragma once

#include <SFML/Audio.hpp>

class Sfx
{
public:
	static const int POOL_SIZE = 10;
public:
	static Sfx& get()
	{
		static Sfx instance;
		return instance;
	}

	void music(bool on);
	void scan();
	void plug();
	void drink();
	void death();

	~Sfx();
private:
	Sfx();

private:
	sf::Sound m_deaths[POOL_SIZE];
	sf::Sound m_drinks[POOL_SIZE];
	sf::Music m_music;
	sf::Sound m_scan, m_plug;
	sf::SoundBuffer m_bufScan, m_bufPlug, m_bufDrink, m_bufDeath;
	int m_currentDeath, m_currentDrink;
};

