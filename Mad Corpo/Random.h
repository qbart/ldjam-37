#pragma once


struct Random
{
	static void init()
	{
		std::srand(std::time(0));
	}

	static int range(int min, int max)
	{
		return min + std::rand() % (max - min + 1);
	}

	static float percent()
	{
		return std::rand() / (float)RAND_MAX;
	}
};
