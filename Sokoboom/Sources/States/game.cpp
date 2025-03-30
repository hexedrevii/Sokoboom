#include "../../Headers/States/game.hpp"

#include "../../Headers/data.hpp"
#include "../../Headers/utilities.hpp"

#include <raymath.h>

#include <cmath>
#include <format>

namespace sokoboom {

void Game::process_player(GameData& data, Tyler& tyler, PlayerVarying& player)
{
	if (IsKeyPressed(KEY_A))
	{
		player.position.x -= 1;
		tyler.tyler_the_creator++;

		this->on_player_moved(data, tyler, player, Direction::left);
	}

	if (IsKeyPressed(KEY_D))
	{
		player.position.x += 1;
		tyler.tyler_the_creator++;

		this->on_player_moved(data, tyler, player, Direction::right);
	}

	if (IsKeyPressed(KEY_W))
	{
		player.position.y -= 1;
		tyler.tyler_the_creator++;
	
		this->on_player_moved(data, tyler, player, Direction::up);
	}

	if (IsKeyPressed(KEY_S))
	{
		player.position.y += 1;
		tyler.tyler_the_creator++;
	
		this->on_player_moved(data, tyler, player, Direction::down);
	}
}

void Game::on_player_moved(GameData& data, Tyler& tyler, PlayerVarying& player, Direction direction)
{
	bool hit_corner = false;
	bool hit_box = false;

	if (this->m_finished)
	{
		// Reached gate.
		// todo: generalize
		if (player.position.x == 9 && (player.position.y == 8 || player.position.y == 7))
		{
			player.locked = true;
			data.total_moves += tyler.tyler_the_creator;

			data.state_handler.set(GameState::end);
		}
	}

	// Player hits wall
	if (
		this->m_map.map.get_at_position(
			player.position.x, player.position.y,
			Map::Layer::solid
		) == Map::CellKind::wall
	)
	{
		switch (direction)
		{
		case Direction::left:
			player.position.x += 1;
			tyler.tyler_the_creator--;

			break;

		case Direction::right:
			player.position.x -= 1;
			tyler.tyler_the_creator--;

			break;

		case Direction::up:
			player.position.y += 1;
			tyler.tyler_the_creator--;

			break;

		case Direction::down:
			player.position.y -= 1;
			tyler.tyler_the_creator--;

			break;
		}

		hit_corner = true;
	}

	if (!this->m_finished)
	{
		// todo: box logic only works for 1 box
		this->m_entities.boxes.each([&](Textured&, Position& box) {
			// Box moves
			if (player.position == box.position)
			{
				switch (direction)
				{
				case Direction::left:
					if (this->m_map.map.get_at_position(box.position.x - 1, box.position.y, Map::Layer::solid) == Map::CellKind::wall)
					{
						player.position.x += 1;
						tyler.tyler_the_creator--;

						hit_box = true;
						break;
					}

					box.position.x -= 1;

					break;

				case Direction::right:
					if (this->m_map.map.get_at_position(box.position.x + 1, box.position.y, Map::Layer::solid) == Map::CellKind::wall)
					{
						player.position.x -= 1;
						tyler.tyler_the_creator--;

						hit_box = true;
						break;
					}

					box.position.x += 1;

					break;

				case Direction::up:
					if (this->m_map.map.get_at_position(box.position.x, box.position.y - 1, Map::Layer::solid) == Map::CellKind::wall)
					{
						player.position.y += 1;
						tyler.tyler_the_creator--;

						hit_box = true;
						break;
					}

					box.position.y -= 1;

					break;

				case Direction::down:
					if (this->m_map.map.get_at_position(box.position.x, box.position.y + 1, Map::Layer::solid) == Map::CellKind::wall)
					{
						player.position.y -= 1;
						tyler.tyler_the_creator--;

						hit_box = true;
						break;
					}

					box.position.y += 1;

					break;
				}
			}

			if (player.position != this->m_undos.back().player_position)
			{
				this->m_undos.emplace_back(player.position, box.position);
			}
		});
	}

	if (!hit_box && !hit_corner)
	{
		if (!data.mute_sfx && !data.mute_move) this->m_move();
	}
}

void Game::undo()
{
	if (this->m_finished) return;

	MoveData last = this->m_undos.back();

	// todo: undo only works for 1 player 1 box
	this->m_entities.players.each([x = last.player_position](PlayerShared&, PlayerVarying& player) { player.position = x; });
	this->m_entities.boxes  .each([x = last.box_position   ](Textured    &, Position     & box   ) { box   .position = x; });

	if (this->m_undos.size() > 1) this->m_undos.pop_back();
}

void Game::awake(GameData& data)
{
	this->m_map = data.maps[data.active_map_index];
	for (std::size_t layerIdx = 0, layerSize = this->m_map.map.layers.size(); layerIdx < layerSize; ++layerIdx)
	{
		auto& layer = this->m_map.map.layers[layerIdx];

		for (std::size_t row = 0, rowSize = layer.size(); row < rowSize; ++row)
		{
			for (std::size_t col = 0, colSize = layer[row].size(); col < colSize; ++col)
			{
				if (layer[row][col] == Map::CellKind::none) continue;

				const TilePosition position(row, col);
				switch (layer[row][col])
				{
					case Map::CellKind::box: {
						this->m_entities.boxes.add(position);
						this->m_map.map.set_at_position(position, layerIdx, Map::CellKind::none);
					} break;

					case Map::CellKind::goal: {
						this->m_entities.goals.add(position);
						this->m_map.map.set_at_position(position, layerIdx, Map::CellKind::none);
					} break;

					case Map::CellKind::player: {
						this->m_entities.players.add(position);
						this->m_map.map.set_at_position(position, layerIdx, Map::CellKind::none);
					} break;
				}
			}
		}
	}

	// todo: undo only works for 1 player 1 box
	this->m_undos.emplace_back();
	this->m_entities.players.each([&m = this->m_undos.back()](PlayerShared&, PlayerVarying& player) { m.player_position = player.position; });
	this->m_entities.boxes  .each([&m = this->m_undos.back()](Textured    &, Position     & box   ) { m.box_position    = box   .position; });

	// Pause UI
	{
		const Vector2 resume_dim = this->m_font.measure_text_ex("resume", 10.0f, 0.1f);
		this->m_buttons.emplace_back(
			this->m_font.get(),
			"resume", 10.0f,
			Vector2((GameData::GAME_SIZE.x - resume_dim.x) / 2, 40),
			[this](Button& /*self*/) {
				this->m_paused = false;
			}
		);
	}

	{
		const Vector2 menu_dim = this->m_font.measure_text_ex("menu", 10.0f, 0.1f);
		this->m_buttons.emplace_back(
			this->m_font.get(),
			"menu", 10.0f,
			Vector2((GameData::GAME_SIZE.x - menu_dim.x) / 2, 50),
			[this, &data](Button& /*self*/) {
				data.active_map_index = 0;
				data.total_moves = 0;

				data.state_handler.set(GameState::menu);
			}
		);
	}
}

void Game::process(GameData& data)
{
	if (IsKeyPressed(KEY_ESCAPE))
	{
		this->m_paused = !this->m_paused;
	}

	if (this->m_paused)
	{
		for (Button& btn : this->m_buttons)
		{
			btn.process(data.virtual_mouse);
		}

		return;
	}

	if (!this->m_finished)
	{
		this->m_ticks++;
	}

	// todo: there's only 1 player
	this->m_entities.players.each(
		[this, &data](PlayerShared& shared, PlayerVarying& player) {
			if (!player.locked) this->process_player(data, shared, player);
		});

	if (IsKeyPressed(KEY_R))
	{
		if (!this->m_switched)
		{
			data.state_handler.set(GameState::game);

			this->m_switched = true;
		}
	}

	// Low budget pressed repeat function
	if (IsKeyDown(KEY_Z))
	{
		if (!this->m_undoing)
		{
			// Remove the last stored movement if its the same position.
			this->m_entities.players.each([&undos = this->m_undos](PlayerShared&, PlayerVarying& player) {
				if (undos.size() > 1 && player.position == undos.back().player_position)
				{
					undos.pop_back();
				}
			});

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
			// todo: very awkward because this logic doesn't make sense for general number of objects
			this->m_entities.visitTylerBoxGoal(
				[this, &data](Tyler& tyler, Position& box, Position& goal) {
					if (box.position != goal.position) return;

					// The end
					if (data.active_map_index == 11)
					{
						this->m_entities.boxes.varying.clear();

						this->m_switched = true;
						this->m_finished = true;

						// Gate
						this->m_map.map.set_at_position(9, 8, Map::Layer::solid, Map::CellKind::none);
						this->m_map.map.set_at_position(9, 7, Map::Layer::solid, Map::CellKind::none);

						if (!data.mute_sfx) this->m_explode();

						return;
					}

					data.total_moves += tyler.tyler_the_creator;

					if (!data.mute_sfx) this->m_next();
					data.active_map_index++;
					data.state_handler.set(GameState::game);

					this->m_switched = true;
				});
		}
	}
}

void Game::render(GameData& /*data*/)
{
	ClearBackground(DARKBLUE);

	this->m_entities.goals  .each([](Textured& tex, Position& p) { tex.texture.draw(Vector2(p.position) * GameData::TILE_SIZE, WHITE); });
	this->m_entities.boxes  .each([](Textured& tex, Position& p) { tex.texture.draw(Vector2(p.position) * GameData::TILE_SIZE, WHITE); });
	this->m_entities.players.each([](Textured& tex, Position& p) { tex.texture.draw(Vector2(p.position) * GameData::TILE_SIZE, WHITE); });
	this->m_map.map.draw(this->m_wall.get());

	this->m_entities.players.each([this](PlayerShared& shared, PlayerVarying& /*player*/) {
		DrawRectangle(
			0, utilities::trunc(GameData::GAME_SIZE.y) - GameData::GAP, utilities::trunc(GameData::GAME_SIZE.x), GameData::GAP, GRAY
		);

		this->m_font.draw_text_pro(
			std::format("{:03} MOVES", shared.tyler_the_creator).c_str(), 
			Vector2(1, GameData::GAME_SIZE.y - GameData::GAP + 1), 
			Vector2Zero(), 0, 5.0f, 0.1f, WHITE
		);
	
		std::string name = std::format("-{}-", this->m_map.name);
		Vector2 name_dim = this->m_font.measure_text_ex(name.c_str(), 5.0f, 0.1f);

		this->m_font.draw_text_pro(
			name.c_str(),
			Vector2(
				floor(GameData::GAME_SIZE.x - name_dim.x), 
				floor(GameData::GAME_SIZE.y - GameData::GAP + 1)
			),
			Vector2Zero(), 0, 5.0f, 0.1f, WHITE
		);
	});

	if (this->m_paused)
	{
		DrawRectangleV(
			Vector2Zero(), GameData::GAME_SIZE,
			Fade(BLACK, 0.7f)
		);

		Vector2 dim = this->m_font.measure_text_ex("PAUSED", 10.0f, 0.1f);
		this->m_font.draw_text_pro(
			"PAUSED",
			Vector2((GameData::GAME_SIZE.x - dim.x) / 2, 5),
			Vector2Zero(),
			0, 10.0f, 0.1f, WHITE
		);

		for (Button& btn : this->m_buttons)
		{
			btn.render();
		}
	}
}

} // namespace sokoboom