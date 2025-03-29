#pragma once

#include <raylib.h>

namespace sokoboom {

class Entity 
{
protected:
	Entity(Vector2 position = Vector2(0, 0))
		: position(position)
	{
	}

public:
	Vector2 position = Vector2(0, 0);

	virtual ~Entity() = default;
	virtual void awake() {}
	virtual void process() {}
	virtual void render() {}
};

} // namespace sokoboom