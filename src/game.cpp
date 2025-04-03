#include "states/game.hpp"

#include "data.hpp"
#include "utilities.hpp"

#include <raymath.h>

#include <cassert>
#include <cmath>
#include <format>

namespace sokoboom {

void Game::process_player(GameData& data)
{
	if (IsKeyPressed(KEY_A)) this->move_player(data, Direction::left );
	if (IsKeyPressed(KEY_D)) this->move_player(data, Direction::right);
	if (IsKeyPressed(KEY_W)) this->move_player(data, Direction::up   );
	if (IsKeyPressed(KEY_S)) this->move_player(data, Direction::down );
}

void Game::move_player(GameData& data, Direction direction)
{
	const auto heading = toVector(direction);
	const auto player_dst = this->m_player.position + heading;
	const auto    box_dst = this->m_box   .position + heading;

	// Player hits wall
	const bool hit_wall = this->m_map.get(player_dst, Map::Layer::solid).is_wall();

	// Box moves
	const bool hit_box = this->m_box_count && player_dst == this->m_box.position;
	const bool hit_box_wall = hit_box
		&& this->m_map.get(box_dst, Map::Layer::solid).is_wall();

	const bool player_moved = !hit_wall && !hit_box_wall;
	if (player_moved)
	{
		auto const box_pos = hit_box ? box_dst : this->m_box.position;
		this->m_player.position = player_dst;
		this->m_box   .position = box_pos;

		if (!this->m_finished)
		{
			this->m_undos.emplace_back(player_dst, box_pos);
			this->m_player.tyler_the_creator += 1;
		}

		data.play_move();
	}

	if (this->m_finished)
	{
		// Reached gate.
		// todo: generalize
		if (this->m_player.position.x == 9 && (this->m_player.position.y == 8 || this->m_player.position.y == 7))
		{
			this->m_player.locked = true;
			data.total_moves += this->m_player.tyler_the_creator;

			data.transition_state(GameState::end);
		}
	}
}

void Game::undo()
{
	if (this->m_finished) return;

	MoveData last = this->m_undos.back();
	this->m_player.position = last.player_position;
	this->m_box   .position = last.box_position   ;

	if (this->m_undos.size() > 1) this->m_undos.pop_back();
}

void Game::awake(GameData& data)
{
	reset(*this);

	std::size_t goals = 0;
	std::size_t boxes = 0;
	std::size_t players = 0;
	this->m_map = data.active_map();
	for (std::size_t layerIdx = 0, layerSize = this->m_map.layers.size(); layerIdx < layerSize; ++layerIdx)
	{
		auto& grid = this->m_map.layers[layerIdx];

		for (Map::Position pos{}; pos.x < grid->width; ++pos.x)
		{
			for (pos.y = 0; pos.y < grid->height; ++pos.y)
			{
				const auto cell = grid->get(pos);
				if (cell.goal)
				{
					this->m_goal.position = pos;
					++goals;
				}
				switch (cell.object)
				{
					case Map::Cell::Object::none: [[fallthrough]];
					case Map::Cell::Object::wall:
						continue;

					case Map::Cell::Object::box   : this->m_box   .position = pos; ++boxes  ; break;
					case Map::Cell::Object::player: this->m_player.position = pos; ++players; break;

					default: unreachable();
				}
			}
		}
	}

	assert(players == 1);
	assert(goals == boxes);
	assert(goals >= 1);

	this->m_box_count = boxes;
	this->m_undos.emplace_back(this->m_player.position, this->m_box.position);

	assert(m_undos.size() >= 1);
}

void Game::process(GameData& data)
{
	if (IsKeyPressed(KEY_ESCAPE))
	{
		data.state_handler.replace(m_pause);
		return;
	}

	if (!this->m_finished) this->m_ticks++;

#ifndef NDEBUG
	if (IsKeyPressed(KEY_T))
	{
		this->m_box.position = this->m_goal.position;
	}
#endif

	if (!this->m_player.locked) this->process_player(data);

	if (IsKeyPressed(KEY_R))
	{
		if (!this->m_switched)
		{
			data.transition_state(GameState::game);

			this->m_switched = true;
		}
	}

	// Low budget pressed repeat function
	if (IsKeyDown(KEY_Z))
	{
		if (!this->m_undoing)
		{
			// Remove the last stored movement if its the same position.
			if (this->m_undos.size() > 1 && this->m_player.position == this->m_undos.back().player_position)
			{
				this->m_undos.pop_back();
			}

			this->undo();
			this->m_undoing = true;
		}
	}
	else
	{
		if (this->m_undoing)
		{
			this->m_time = 0;
			this->m_undoing = false;
			this->m_undo_delay = 0.35f;
		}
	}

	if (this->m_undoing)
	{
		this->m_time += GetFrameTime();
		if (this->m_time >= this->m_undo_delay)
		{
			this->m_time = 0;
			this->m_undo_delay = 0.05f;
			this->undo();
		}
	}

	// todo: this whole ticks counter thing doens't make any sense
	if (this->m_ticks == 30 && !this->m_finished)
	{
		this->m_ticks = 0;

		if (!this->m_switched)
		{
			if (this->m_box.position != this->m_goal.position) return;

			// The end
			if (data.active_map_index == 11)
			{
				this->m_box_count = 0;

				this->m_switched = true;
				this->m_finished = true;

				// Gate
				this->m_map.set({9, 8}, Map::Layer::solid, Map::Cell{});
				this->m_map.set({9, 7}, Map::Layer::solid, Map::Cell{});

				data.play_explode();

				return;
			}

			data.total_moves += this->m_player.tyler_the_creator;

			data.play_next();

			data.active_map_index++;
			data.transition_state(GameState::game);

			this->m_switched = true;
		}
	}
}

void Game::render(GameData& data)
{
	ClearBackground(DARKBLUE);

	DrawTextureV(resource[Resource::fixed::Texture::goal], Vector2(this->m_goal.position) * GameData::TILE_SIZE, WHITE);
	if (this->m_box_count) DrawTextureV(resource[Resource::fixed::Texture::box], Vector2(this->m_box.position) * GameData::TILE_SIZE, WHITE);
	DrawTextureV(resource[Resource::fixed::Texture::player], Vector2(this->m_player.position) * GameData::TILE_SIZE, WHITE);
	this->m_map.draw(resource[Resource::fixed::Texture::wall]);

	DrawRectangle(0, trunc(GameData::GAME_SIZE.y) - GameData::GAP, trunc(GameData::GAME_SIZE.x), GameData::GAP, GRAY);

	DrawTextPro(
		resource[font],
		std::format("{:03} MOVES", this->m_player.tyler_the_creator).c_str(),
		Vector2(1, GameData::GAME_SIZE.y - GameData::GAP + 1),
		Vector2Zero(), 0, 5.0f, 0.1f, WHITE
	);
	
	std::string name = std::format("-{}-", data.active_map_name());
	Vector2 name_dim = MeasureTextEx(resource[font], name.c_str(), 5.0f, 0.1f);

	DrawTextPro(
		resource[font],
		name.c_str(),
		Vector2(floor(GameData::GAME_SIZE.x - name_dim.x), floor(GameData::GAME_SIZE.y - GameData::GAP + 1)),
		Vector2Zero(), 0, 5.0f, 0.1f, WHITE
	);
}

} // namespace sokoboom