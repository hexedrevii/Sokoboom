#pragma once

#include <raylib.h>

namespace sokoboom {

class Entity 
{
public:
	int id;

	Vector2 position = Vector2(0, 0);
	Vector2 velocity = Vector2(0, 0);

	Entity(int id, Vector2 position = Vector2(0, 0), Vector2 velocity = Vector2(0, 0))
		: id(id)
		, position(position)
		, velocity(velocity)
	{
	}

	virtual void awake() {}

	virtual void process() {}
	virtual void render() {}
};

} // namespace sokoboom