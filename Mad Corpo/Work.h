#pragma once

#include "Node.h"
#include "Types.h"


class Person;
class Server;

class Work : public Node
{
public:
	Work();
	virtual ~Work();

private:
	virtual void onAdd();

private:
	Server *m_server;
	Person *m_people[MAX_PEOPLE];
};

