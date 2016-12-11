#include "Player.h"
#include "Types.h"
#include "Events.h"


const float VELOCITY = TILE_SIZE * 10; // pixels per second

Player::Player() :
	Node(),
	PositionableOnGrid(this),
	WalkableOnGrid(this),
	m_shape(12.f),
	m_action(0),
	m_direction(NONE),
	m_state(STAY)
{
	setOrigin(sf::Vector2f(-5, -5));
	m_shape.setFillColor(sf::Color::Green);
	Grid::Data data = {
		{ 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 3, 0, 0 },
		{ 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1 },
		{ 1, 1, 4, 4, 4, 1, 1, 4, 4, 1, 1, 4, 4, 4, 1, 1 },
		{ 1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 1 },
		{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
		{ 1, 1, 4, 4, 4, 1, 1, 4, 4, 1, 1, 4, 4, 4, 1, 1 },
		{ 1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 1 },
		{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
		{ 1, 1, 4, 4, 4, 1, 1, 4, 4, 1, 1, 4, 4, 4, 1, 1 },
		{ 1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 1 },
		{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
		{ 1, 1, 4, 4, 4, 1, 1, 4, 4, 1, 1, 4, 4, 4, 1, 1 },
		{ 1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 1 },
		{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
		{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
		{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 }
	};
	m_grid = data;

	Events::get().watch("system.scan.stop", OnNotify(Player::onScanStop));
	Events::get().observe("input.keyReleased", OnEvent(Player::onKeyReleased));
}

Player::~Player()
{
}

void Player::walkingDone()
{
	m_state = STAY;
	m_direction = NONE;

	if (m_serverPlugged)
	{
		CableEvent event;
		event.action = CableAction::PLUG_ROLL;
		event.position = m_gridPosition;
		Event data;
		data.cable = event;
		Events::get().send("server.cable.put", &data);
	}
}

void Player::updateCurrent(sf::Time dt)
{
	if (m_state == STAY)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			m_direction = DOWN;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			m_direction = UP;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			m_direction = LEFT;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			m_direction = RIGHT;
		}

		switch (m_direction)
		{
		case UP:
			m_target.x = m_gridPosition.x;
			m_target.y = m_gridPosition.y - 1;
			break;

		case DOWN:
			m_target.x = m_gridPosition.x;
			m_target.y = m_gridPosition.y + 1;
			break;

		case LEFT:
			m_target.x = m_gridPosition.x - 1;
			m_target.y = m_gridPosition.y;
			break;

		case RIGHT:
			m_target.x = m_gridPosition.x + 1;
			m_target.y = m_gridPosition.y;
			break;
		}

		if (m_grid.walkable(m_target.x, m_target.y) && m_direction != NONE)
		{
			m_state = WALK;
		}
		else
		{
			m_direction = NONE;
		}
	}

	if (m_state == WALK)
	{
		walk(dt, VELOCITY);
	}
}

void Player::drawCurrent(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(m_shape, states);
}

void Player::onKeyReleased(Event * event)
{
	auto code = event->key.code;

	if (code == sf::Keyboard::Space)
	{
		if (m_state == STAY)
		{
			if (m_grid.actionable(m_gridPosition.x, m_gridPosition.y))
			{
				m_action = m_grid.at(m_gridPosition);

				switch (m_action)
				{
				case TILE_SEAT:
					m_state = SCAN;
					Events::get().notify("system.scan.start");
					m_state = STAY;
					break;

				case TILE_SERVER:
					m_state = ROLL_CABLE;
					if (!m_serverPlugged)
					{
						Events::get().notify("server.plug.in");

						CableEvent event;
						event.action = CableAction::PLUG_IN;
						event.position = m_gridPosition;
						Event data;
						data.cable = event;
						Events::get().send("server.cable.put", &data);
					}
					else
					{
						Events::get().notify("server.plug.out");

						CableEvent event;
						event.action = CableAction::PLUG_OUT;
						event.position = m_gridPosition;
						Event data;
						data.cable = event;
						Events::get().send("server.cable.put", &data);
					}
					m_serverPlugged = !m_serverPlugged;
					m_state = STAY;
					break;

				case TILE_CONNECT:
					if (m_serverPlugged)
					{
						Events::get().notify("server.plug.connect");

						CableEvent event;
						event.action = CableAction::PLUG_CONNECT;
						event.position = m_gridPosition;
						event.position.y += 1;
						Event data;
						data.cable = event;
						Events::get().send("server.cable.put", &data);
						m_serverPlugged = false;
					}
					break;
				}
			}
		}
	}
}

void Player::onScanStop()
{
#ifdef DEBUG
	printf("Events: Player#onScanStop\n");
#endif // DEBUG
	//m_state = STAY;
}
