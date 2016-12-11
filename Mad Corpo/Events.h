#pragma once

#include <string>
#include <vector>
#include <map>
#include <functional>
#include "Types.h"
#include <SFML/Window/Event.hpp>
#include "GridPosition.h"


#define OnEvent(_method) std::bind(&_method, this, std::placeholders::_1)
#define OnNotify(_method) std::bind(&_method, this)

typedef enum { PLUG_IN, PLUG_OUT, PLUG_ROLL, PLUG_CONNECT } CableAction;

typedef struct
{
	GridPosition position;
} WalkEvent;

typedef struct
{
	GridPosition position;
	CableAction action;
} CableEvent;

typedef union Event
{
	sf::Event::KeyEvent key;
	CableEvent cable;
	WalkEvent walk;

	Event() {};
	~Event() {};
} Event;

class Events
{
public:
	typedef std::function<void()> Callback;
	typedef std::function<void(Event *event)> CustomCallback;
	typedef std::vector<Callback> CallbackList;
	typedef std::vector<CustomCallback> CustomCallbackList;
	
public:
	static Events& get()
	{
		static Events instance;
		return instance;
	}
	virtual ~Events();

	void watch(const std::string& name, const Callback& callback);
	void observe(const std::string& name, const CustomCallback& callback);
	void notify(const std::string& name);
	void send(const std::string& name, Event* data);

private:
	Events();

private:
	std::map<std::string, CallbackList> m_registered;
	std::map<std::string, CustomCallbackList> m_registeredCustom;
};

