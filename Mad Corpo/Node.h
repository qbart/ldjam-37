#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

class Node : public sf::Drawable, public sf::Transformable, private sf::NonCopyable
{
public:
	typedef std::unique_ptr<Node> Ptr;

public:
	Node();
	virtual ~Node();
	void addChild(Ptr child);
	void addChild(Node* child);
	Ptr removeChild(const Node& node);
	virtual void update(sf::Time dt);
	sf::Transform getWorldTransform() const;
	sf::Vector2f getWorldPosition() const;

private:
	virtual void updateCurrent(sf::Time dt);
	virtual void updateChildren(sf::Time dt);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	std::vector<Ptr> m_children;
	Node *m_parent = nullptr;
};

