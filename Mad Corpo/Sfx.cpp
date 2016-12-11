#include "Sfx.h"
#include <cassert>
#include "Events.h"


void Sfx::music(bool on)
{
	if (on)
		m_music.play();
	else
		m_music.stop();

	Events::get().watch("system.scan.start", OnNotify(Sfx::scan));
	Events::get().watch("server.plug.in", OnNotify(Sfx::plug));
	Events::get().watch("server.plug.out", OnNotify(Sfx::plug));
	Events::get().watch("server.plug.connect", OnNotify(Sfx::plug));
	Events::get().watch("coffee.drink", OnNotify(Sfx::drink));
	Events::get().watch("person.death", OnNotify(Sfx::death));
}

void Sfx::scan()
{
	m_scan.play();
}

void Sfx::plug()
{
	m_plug.play();
}

void Sfx::drink()
{
	m_drinks[m_currentDrink].play();

	++m_currentDrink;
	if (m_currentDrink >= POOL_SIZE)
		m_currentDrink = 0;
}

void Sfx::death()
{
	m_deaths[m_currentDeath].play();

	++m_currentDeath;
	if (m_currentDeath >= POOL_SIZE)
		m_currentDeath = 0;
}

Sfx::~Sfx()
{
}

Sfx::Sfx()
{
	assert(m_music.openFromFile("assets/background.wav"));
	assert(m_bufScan.loadFromFile("assets/scan.wav"));
	assert(m_bufPlug.loadFromFile("assets/plug.wav"));
	assert(m_bufDrink.loadFromFile("assets/drink.wav"));
	assert(m_bufDeath.loadFromFile("assets/death.wav"));
	m_scan.setBuffer(m_bufScan);
	m_plug.setBuffer(m_bufPlug);
	for (int i = 0; i < POOL_SIZE; ++i)
	{
		m_drinks[i].setBuffer(m_bufDrink);
		m_deaths[i].setBuffer(m_bufDeath);
	}
	m_music.setLoop(true);
	m_music.setVolume(60);

	m_currentDrink = 0;
	m_currentDeath = 0;
}
