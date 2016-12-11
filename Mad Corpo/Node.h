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
	bool isVisible() const { return m_visible; }
	void setVisible(bool visible);
	bool isActive() const { return m_active; }
	void setActive(bool active);
	void addChild(Ptr child);
	void addChild(Node* child);
	Ptr removeChild(const Node& node);
	void removeChildLater(Node* node);
	virtual void update(sf::Time dt);
	sf::Transform getWorldTransform() const;
	sf::Vector2f getWorldPosition() const;

private:
	virtual void onAdd();
	virtual void updateCurrent(sf::Time dt);
	virtual void updateChildren(sf::Time dt);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	std::vector<Ptr> m_children;
	std::vector<Node*> m_removalQueue;
	Node *m_parent = nullptr;
	bool m_visible;
	bool m_active;
};

