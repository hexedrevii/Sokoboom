#include "../../Headers/States/game.hpp"

#include "../../Headers/data.hpp"
#include "../../Headers/utilities.hpp"

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
	const bool hit_wall = this->m_map.map.get_at_position(player_dst, Map::Layer::solid) == Map::CellKind::wall;

	// Box moves
	const bool hit_box = this->m_box_count && player_dst == this->m_box.position;
	const bool hit_box_wall = hit_box
		&& this->m_map.map.get_at_position(box_dst, Map::Layer::solid) == Map::CellKind::wall;

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

			data.change_state(GameState::end);
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

Game::Game(GameData& data)
{
	this->m_map = data.maps[data.active_map_index];

	std::size_t goals = 0;
	std::size_t boxes = 0;
	std::size_t players = 0;
	for (std::size_t layerIdx = 0, layerSize = this->m_map.map.layers.size(); layerIdx < layerSize; ++layerIdx)
	{
		auto& layer = this->m_map.map.layers[layerIdx];
		auto& map = this->m_map.map;

		Map::Position pos;
		const int rowSize = shrink(layer.size());
		for (pos.x = 0; pos.x < rowSize; ++pos.x)
		{
			const int colSize = shrink(layer[pos.x].size());
			for (pos.y = 0; pos.y < colSize; ++pos.y)
			{
				switch (map.get_at_position(pos, layerIdx))
				{
					case Map::CellKind::none: [[fallthrough]];
					case Map::CellKind::wall:
						continue;

					case Map::CellKind::box   : this->m_box   .position = pos; ++boxes  ; break;
					case Map::CellKind::goal  : this->m_goal  .position = pos; ++goals  ; break;
					case Map::CellKind::player: this->m_player.position = pos; ++players; break;

					default: unreachable();
				}
				map.set_at_position(pos, layerIdx, Map::CellKind::none);
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
		this->m_paused = !this->m_paused;
	}

	if (this->m_paused)
	{
		for (auto& btn : this->m_buttons)
		{
			btn.process(data, *this);
		}

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
			data.change_state(GameState::game);

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
				this->m_map.map.set_at_position(9, 8, Map::Layer::solid, Map::CellKind::none);
				this->m_map.map.set_at_position(9, 7, Map::Layer::solid, Map::CellKind::none);

				data.play_explode();

				return;
			}

			data.total_moves += this->m_player.tyler_the_creator;

			data.play_next();

			data.active_map_index++;
			data.change_state(GameState::game);

			this->m_switched = true;
		}
	}
}

void Game::render(GameData& /*data*/)
{
	ClearBackground(DARKBLUE);

	DrawTextureV(resource[Resource::fixed::Texture::goal], Vector2(this->m_goal.position) * GameData::TILE_SIZE, WHITE);
	if (this->m_box_count) DrawTextureV(resource[Resource::fixed::Texture::box], Vector2(this->m_box.position) * GameData::TILE_SIZE, WHITE);
	DrawTextureV(resource[Resource::fixed::Texture::player], Vector2(this->m_player.position) * GameData::TILE_SIZE, WHITE);
	this->m_map.map.draw(resource[Resource::fixed::Texture::wall]);

	DrawRectangle(0, trunc(GameData::GAME_SIZE.y) - GameData::GAP, trunc(GameData::GAME_SIZE.x), GameData::GAP, GRAY);

	DrawTextPro(
		resource[font],
		std::format("{:03} MOVES", this->m_player.tyler_the_creator).c_str(),
		Vector2(1, GameData::GAME_SIZE.y - GameData::GAP + 1),
		Vector2Zero(), 0, 5.0f, 0.1f, WHITE
	);
	
	std::string name = std::format("-{}-", this->m_map.name);
	Vector2 name_dim = MeasureTextEx(resource[font], name.c_str(), 5.0f, 0.1f);

	DrawTextPro(
		resource[font],
		name.c_str(),
		Vector2(floor(GameData::GAME_SIZE.x - name_dim.x), floor(GameData::GAME_SIZE.y - GameData::GAP + 1)),
		Vector2Zero(), 0, 5.0f, 0.1f, WHITE
	);

	if (this->m_paused)
	{
		DrawRectangleV(
			Vector2Zero(), GameData::GAME_SIZE,
			Fade(BLACK, 0.7f)
		);

		Vector2 dim = MeasureTextEx(resource[font], "PAUSED", 10.0f, 0.1f);
		DrawTextPro(
			resource[font],
			"PAUSED",
			Vector2((GameData::GAME_SIZE.x - dim.x) / 2, 5),
			Vector2Zero(),
			0, 10.0f, 0.1f, WHITE
		);

		for (auto& btn : this->m_buttons)
		{
			btn.render();
		}
	}
}

} // namespace sokoboom