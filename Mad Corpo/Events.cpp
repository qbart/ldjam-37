#include "Events.h"



Events::Events()
{
}


Events::~Events()
{
}

void Events::watch(const std::string & name, const Callback & callback)
{
	m_registered[name].push_back(callback);
}

void Events::observe(const std::string & name, const CustomCallback & callback)
{
	m_registeredCustom[name].push_back(callback);
}

void Events::notify(const std::string & name)
{
	for (auto& callback : m_registered[name])
	{
		callback();
	}
}

void Events::send(const std::string & name, Event * data)
{
	for (auto& callback : m_registeredCustom[name])
	{
		callback(data);
	}
}
