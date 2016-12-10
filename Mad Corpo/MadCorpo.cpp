/*
  Mad Corpo

  Made for Ludum Dare #37 (One room).

  Author: Bart³omiej Wójtowicz
  GitHub: https://github.com/qbart/ldjam-37
*/

#include <windows.h>
#include "Game.h"


int main(char argc, char *argv[])
{
#ifndef DEBUG
	FreeConsole();
#endif
	
	Game game;
	return game.run();
}