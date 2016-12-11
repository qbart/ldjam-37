#include "Cable.h"
#include <cassert>
#include "Player.h"
#include "Computer.h"
#include "Random.h"


CablePart::CablePart() : 
	SpriteNode(Texture::CABLE),
	PositionableOnGrid(this)
{
	setOrigin(sf::Vector2f(0, 0));
	setType(HORIZONTAL);
	m_variant = Random::range(0, 1);
}

CablePart::~CablePart()
{
}

void CablePart::setType(ID type)
{
	m_type = type;
	m_sprite.setTextureRect(sf::IntRect(32 * type, m_variant * 32, 32, 32));
}

Cable::Cable() :
	Node(),
	m_broken(false),
	m_computer(nullptr)
{
	static int _id = 0;
	++_id;
	m_key = _id;
}

Cable::~Cable()
{
}

void Cable::disconnectAt(const GridPosition& position)
{
	m_broken = true;
	assert(m_computer != nullptr);
	m_computer->disconnect();
}

void Cable::buildDefaultTopology(const GridPosition& connectorPosition)
{
	GridPosition position = connectorPosition;
	position.y -= 1;

	buildPart(position)->setType(CablePart::BOTTOM_TO_RIGHT);

	while (position.x <= 12)
	{
		position.x += 1;
		buildPart(position)->setType(CablePart::HORIZONTAL);
	}

	position.x += 1;
	buildPart(position)->setType(CablePart::LEFT_TO_TOP);

	while (position.y >= 3)
	{
		position.y -= 1;
		buildPart(position)->setType(CablePart::VERTICAL);
	}

	position.y -= 1;
	buildPart(position)->setType(CablePart::BOTTOM_TO_LEFT);

	position.x -= 1;
	buildPart(position)->setType(CablePart::RIGHT_TO_TOP);

	position.y -= 1;
	buildPart(position)->setType(CablePart::BOTTOM_TO_RIGHT);

	// reverse parts so first part is at the server panel
	std::reverse(m_parts.begin(), m_parts.end());

	for (auto part : m_parts)
		addChild(part);
}

void Cable::put(const GridPosition & position)
{
	bool hasPrevious = m_parts.size() >= 2;

	if (!hasPrevious)
	{ // first part
		auto part = buildPart(position);
		addChild(part);
	}
	else
	{ // subsequent parts
		auto prev = m_parts.at(m_parts.size() - 2);

		if (prev->getGridPosition() == position)
		{ // step back
			auto last = m_parts.back();
			m_parts.pop_back();
			removeChild(*last);
		}
		else
		{ // step forward
			auto part = buildPart(position);
			addChild(part);
		}
	}

	assert(m_parts.size() != 0);

	// adjust type
	switch (m_parts.size())
	{
	case 1:
		m_parts.at(0)->setType(CablePart::RIGHT_TO_LEFT_SHORT);
		break;

	case 2:
		if (m_parts.at(1)->getGridPosition().y == 0)
		{
			m_parts.at(0)->setType(CablePart::HORIZONTAL);
			m_parts.at(1)->setType(CablePart::RIGHT_TO_LEFT_SHORT);
		}
		else
		{
			m_parts.at(0)->setType(CablePart::RIGHT_TO_BOTTOM);
			m_parts.at(1)->setType(CablePart::TOP_TO_BOTTOM_SHORT);
		}
		break;

	default:
		auto c = m_parts.at(m_parts.size() - 1)->getGridPosition();
		auto b = m_parts.at(m_parts.size() - 2)->getGridPosition();
		auto a = m_parts.at(m_parts.size() - 3)->getGridPosition();

		auto turnDiff = c - a;
		auto directionDiff = c - b;

		Player::Direction direction;

		if (directionDiff.x == 1)
		{
			direction = Player::Direction::RIGHT;
			m_parts.at(m_parts.size() - 1)->setType(CablePart::LEFT_TO_RIGHT_SHORT);
		}
		else if (directionDiff.x == -1)
		{
			direction = Player::Direction::LEFT;
			m_parts.at(m_parts.size() - 1)->setType(CablePart::RIGHT_TO_LEFT_SHORT);
		}
		else if (directionDiff.y == -1)
		{
			direction = Player::Direction::UP;
			m_parts.at(m_parts.size() - 1)->setType(CablePart::BOTTOM_TO_TOP_SHORT);
		}
		else if (directionDiff.y == 1)
		{
			direction = Player::Direction::DOWN;
			m_parts.at(m_parts.size() - 1)->setType(CablePart::TOP_TO_BOTTOM_SHORT);
		}

		if (turnDiff.x == 0)
			m_parts.at(m_parts.size() - 2)->setType(CablePart::VERTICAL);

		else if (turnDiff.y == 0)
			m_parts.at(m_parts.size() - 2)->setType(CablePart::HORIZONTAL);

		else if (turnDiff.x > 0 && turnDiff.y > 0 && direction == Player::Direction::RIGHT)
			m_parts.at(m_parts.size() - 2)->setType(CablePart::TOP_TO_RIGHT);
		else if (turnDiff.x > 0 && turnDiff.y > 0 && direction == Player::Direction::DOWN)
			m_parts.at(m_parts.size() - 2)->setType(CablePart::LEFT_TO_BOTTOM);

		else if (turnDiff.x > 0 && turnDiff.y < 0 && direction == Player::Direction::RIGHT)
			m_parts.at(m_parts.size() - 2)->setType(CablePart::BOTTOM_TO_RIGHT);
		else if (turnDiff.x > 0 && turnDiff.y < 0 && direction == Player::Direction::UP)
			m_parts.at(m_parts.size() - 2)->setType(CablePart::LEFT_TO_TOP);

		else if (turnDiff.x < 0 && turnDiff.y > 0 && direction == Player::Direction::LEFT)
			m_parts.at(m_parts.size() - 2)->setType(CablePart::TOP_TO_LEFT);
		else if (turnDiff.x < 0 && turnDiff.y > 0 && direction == Player::Direction::DOWN)
			m_parts.at(m_parts.size() - 2)->setType(CablePart::RIGHT_TO_BOTTOM);

		else if (turnDiff.x < 0 && turnDiff.y < 0 && direction == Player::Direction::LEFT)
			m_parts.at(m_parts.size() - 2)->setType(CablePart::BOTTOM_TO_LEFT);
		else if (turnDiff.x < 0 && turnDiff.y < 0 && direction == Player::Direction::UP)
			m_parts.at(m_parts.size() - 2)->setType(CablePart::RIGHT_TO_TOP);

		else
			assert(false); // that should never happen if all cases are handled

		break;
	}
}

void Cable::setComputer(Computer * computer)
{
	assert(computer != nullptr);
	m_computer = computer;
	computer->connect();

	if (m_parts.size() >= 2)
	{
		auto b = m_parts.at(m_parts.size() - 2)->getGridPosition();
		auto a = m_parts.at(m_parts.size() - 1)->getGridPosition();
		auto d = b - a;

		if (d.y < 0)
			m_parts.back()->setType(CablePart::VERTICAL);
		else if (d.x > 0)
			m_parts.back()->setType(CablePart::BOTTOM_TO_RIGHT);
		if (d.x < 0)
			m_parts.back()->setType(CablePart::BOTTOM_TO_LEFT);
	}
}

CablePart* Cable::buildPart(const GridPosition & at)
{
	auto part = new CablePart();
	part->setGridPosition(at);
	m_parts.push_back(part);

	return part;
}
