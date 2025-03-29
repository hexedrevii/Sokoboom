#pragma once

#include <raylib.h>

namespace sokoboom {

class Entity 
{
public:
	Vector2 position = Vector2(0, 0);

	Entity(Vector2 position = Vector2(0, 0))
		: position(position)
	{
	}

	virtual void awake() {}

	virtual void process() {}
	virtual void render() {}
};

} // namespace sokoboom