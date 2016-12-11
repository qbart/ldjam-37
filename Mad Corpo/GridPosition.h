#pragma once


class GridPosition
{
public:
	static GridPosition ZERO;

	int x = 0;
	int y = 0;

public:
	GridPosition(int x = 0, int y = 0);
	~GridPosition();
	GridPosition stepDown();
	GridPosition stepUp();
	GridPosition stepLeft();
	GridPosition stepRight();
	const int key() const { return y * 1000 + x; }

	GridPosition& operator=(const GridPosition& other)
	{
		x = other.x;
		y = other.y;

		return *this;
	}

	bool operator==(const GridPosition& other)
	{
		return x == other.x && y == other.y;
	}

	bool operator!=(const GridPosition& other)
	{
		return !(*this == other);
	}

	friend const GridPosition operator-(const GridPosition& a, const GridPosition& b)
	{
		GridPosition position;
		position.x = a.x - b.x;
		position.y = a.y - b.y;

		return position;
	}

};