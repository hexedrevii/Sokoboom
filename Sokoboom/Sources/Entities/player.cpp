#include "../../Headers/Entities/player.hpp"

#include "../../Headers/data.hpp"
#include "../../Headers/utilities.hpp"

namespace sokoboom {

Player::Player(int id, Vector2 position, Vector2 velocity)
	: Entity(id, position, velocity)
	, m_sprite(resource.texture2d("Content/Entities/player.png"))
{
}

void Player::process()
{
	if (this->locked) return;

	if (IsKeyPressed(KEY_A))
	{
		this->position.x -= GameData::TILE_SIZE;
		this->tyler_the_creator++;

		if (this->on_player_moved != nullptr)
		{
			this->on_player_moved(this->position, Direction::LEFT);
		}
	}

	if (IsKeyPressed(KEY_D))
	{
		this->position.x += GameData::TILE_SIZE;
		this->tyler_the_creator++;

		if (this->on_player_moved != nullptr)
		{
			this->on_player_moved(this->position, Direction::RIGHT);
		}
	}

	if (IsKeyPressed(KEY_W))
	{
		this->position.y -= GameData::TILE_SIZE;
		this->tyler_the_creator++;
	
		if (this->on_player_moved != nullptr)
		{
			this->on_player_moved(this->position, Direction::UP);
		}
	}

	if (IsKeyPressed(KEY_S))
	{
		this->position.y += GameData::TILE_SIZE;
		this->tyler_the_creator++;
	
		if (this->on_player_moved != nullptr)
		{
			this->on_player_moved(this->position, Direction::DOWN);
		}
	}
}

void Player::render()
{
	this->m_sprite.draw(this->position, WHITE);
}

} // namespace sokoboom