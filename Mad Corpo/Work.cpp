#include "Work.h"
#include "Person.h"
#include "Events.h"
#include "Server.h"


Work::Work()
{
}

Work::~Work()
{
}

void Work::onAdd()
{
	m_server = new Server();
	addChild(m_server);

	for (int i = 0; i < MAX_PEOPLE; ++i)
	{
		auto person = new Person((*m_server)[i]);
		m_people[i] = person;
		addChild(person);
	}

}
