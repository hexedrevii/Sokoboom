#pragma once

#include "../map.hpp"
#include "../resource.hpp"

#include <raylib.h>

#include <vector>

namespace sokoboom {

#define SOKOBOOM_X_DIRECTIONS(X)\
	X(left , (-1, +0))\
	X(right, (+1, +0))\
	X(up   , (+0, -1))\
	X(down , (+0, +1))

enum class Direction
{
#define X(name, vec) name,
	SOKOBOOM_X_DIRECTIONS(X)
#undef X
};

constexpr Map::Position DirectionVector[] {
#define X(name, vec) Map::Position vec,
	SOKOBOOM_X_DIRECTIONS(X)
#undef X
};

constexpr int2 toVector(Direction dir) noexcept
{
	return DirectionVector[std::size_t(dir)];
}

struct Textured
{
	Texture2D texture;
};

struct Position
{
	Map::Position position;
};

struct Tyler
{
	int tyler_the_creator = 0;
};

struct Lockable
{
	bool locked = false;
};

} // namespace sokoboom