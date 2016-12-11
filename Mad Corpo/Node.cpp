#include "Node.h"
#include <algorithm>
#include <cassert>

Node::Node() :
	m_visible(true),
	m_active(true)
{
}

Node::~Node()
{
}

void Node::setVisible(bool visible)
{
	m_visible = visible;
}

void Node::setActive(bool active)
{
	m_active = active;
}

void Node::addChild(Node::Ptr child)
{
	child->m_parent = this;
	child->onAdd();
	m_children.push_back(std::move(child));
}

void Node::addChild(Node* child)
{
	Node::Ptr ptr(child);
	addChild(std::move(ptr));
}

Node::Ptr Node::removeChild(const Node& node)
{
	auto found = std::find_if(m_children.begin(), m_children.end(), [&](Ptr& ptr) -> bool
	{
		return ptr.get() == &node;
	});

	assert(found != m_children.end());

	Ptr result = std::move(*found);
	result->m_parent = nullptr;
	m_children.erase(found);
	return result;
}

void Node::removeChildLater(Node * node)
{
	m_removalQueue.push_back(node);
}

void Node::onAdd()
{
}

void Node::update(sf::Time dt)
{
	while (!m_removalQueue.empty())
	{
		auto child = m_removalQueue.back();
		m_removalQueue.pop_back();
		removeChild(*child);
	}

	if (m_active)
	{
		updateCurrent(dt);
		updateChildren(dt);
	}
}

sf::Transform Node::getWorldTransform() const
{
	auto transform = sf::Transform::Identity;

	for (const Node* node = this; node != nullptr; node = node->m_parent)
	{
		transform = node->getTransform() * transform;
	}

	return transform;
}

sf::Vector2f Node::getWorldPosition() const
{
	return getWorldTransform() * sf::Vector2f();
}

void Node::updateCurrent(sf::Time dt)
{
}

void Node::updateChildren(sf::Time dt)
{
	for (Ptr& child : m_children)
	{
		child->update(dt);
	}
}

void Node::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (m_visible)
	{
		states.transform *= getTransform();

		drawCurrent(target, states);

		for (const Ptr& child : m_children)
		{
			child->draw(target, states);
		}
	}
}

void Node::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{

}
