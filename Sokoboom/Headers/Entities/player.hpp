#pragma once

#include "../resource.hpp"
#include "entity.hpp"

#include <raylib.h>

#include <functional>

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

	std::function<void(Vector2, Direction)> m_on_player_moved;
	void on_move(Direction direction)
	{
		this->m_on_player_moved(this->position, direction);
	}

	friend class EntityController;
	Player(std::function<void(Vector2, Direction)> on_player_move, Vector2 position = Vector2(0, 0));

public:
	int tyler_the_creator = 0;

	bool locked = false;

	void process() override;
	void render() override;
};

} // namespace sokoboom