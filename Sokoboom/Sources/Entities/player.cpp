#include "../../Headers/Entities/player.hpp"

#include "../../Headers/data.hpp"
#include "../../Headers/utilities.hpp"

namespace sokoboom {

Player::Player(std::function<void(Vector2, Direction)> on_player_move, Vector2 position)
	: Entity(position)
	, m_sprite(resource.texture2d("Content/Entities/player.png"))
	, m_on_player_moved(std::move(on_player_move))
{
}

void Player::process()
{
	if (this->locked) return;

	if (IsKeyPressed(KEY_A))
	{
		this->position.x -= GameData::TILE_SIZE;
		this->tyler_the_creator++;

		this->on_move(Direction::left);
	}

	if (IsKeyPressed(KEY_D))
	{
		this->position.x += GameData::TILE_SIZE;
		this->tyler_the_creator++;

		this->on_move(Direction::right);
	}

	if (IsKeyPressed(KEY_W))
	{
		this->position.y -= GameData::TILE_SIZE;
		this->tyler_the_creator++;
	
		this->on_move(Direction::up);
	}

	if (IsKeyPressed(KEY_S))
	{
		this->position.y += GameData::TILE_SIZE;
		this->tyler_the_creator++;
	
		this->on_move(Direction::down);
	}
}

void Player::render()
{
	this->m_sprite.draw(this->position, WHITE);
}

} // namespace sokoboom