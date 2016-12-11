#pragma once


static const int WIDTH = 1024;
static const int HEIGHT = 768;

static const int TILE_SIZE = 32;
static const int TILES_COUNT = 16;

typedef unsigned char u8;

static const u8 TILE_INVALID = -1;
static const u8 TILE_WALK = 1;
static const u8 TILE_SEAT = 2;
static const u8 TILE_SERVER = 3;
static const u8 TILE_CONNECT = 4;

static const int MAX_PEOPLE = 32;

template<class T>
constexpr const T& clamp(const T& v, const T& lo, const T& hi)
{
	return std::max(std::min(v, hi), lo);
}
