#pragma once

#include "../resource.hpp"
#include "entity.hpp"

#include <raylib.h>

namespace sokoboom {

struct GameData;

enum class Direction
{
	left,
	right,
	up,
	down,
};

class Player : public Entity
{
private:
	Texture2D m_sprite;

	friend class EntityController;
	explicit Player(Vector2 position = Vector2(0, 0))
		: Entity(position)
		, m_sprite(resource.texture2d("Content/Entities/player.png"))
	{
	}

public:
	int tyler_the_creator = 0;

	bool locked = false;

	void render() override
	{
		this->m_sprite.draw(this->position, WHITE);
	}
};

} // namespace sokoboom