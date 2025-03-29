#include "../../Headers/States/game.hpp"

#include "../../Headers/data.hpp"
#include "../../Headers/utilities.hpp"

#include <raymath.h>

#include <cmath>
#include <format>

namespace sokoboom {

static constexpr int BOX_ID = 2;
static constexpr int GOAL_ID = 3;
static constexpr int PLAYER_ID = 4;

static constexpr int SOLID_LAYER = 0;
static constexpr int SOLID_WALL = 1;

Game::Game()
	: m_font(resource.font("Content/pico-8.ttf"))
	, m_move(resource.sound("Content/Audio/move.wav"))
	, m_next(resource.sound("Content/Audio/next.wav"))
	, m_explode(resource.sound("Content/Audio/explosion.wav"))
{
}

void Game::process_player(GameData& data, Tyler& tyler, PlayerVarying& player)
{
	if (IsKeyPressed(KEY_A))
	{
		player.position.x -= GameData::TILE_SIZE;
		tyler.tyler_the_creator++;

		this->on_player_moved(data, tyler, player, Direction::left);
	}

	if (IsKeyPressed(KEY_D))
	{
		player.position.x += GameData::TILE_SIZE;
		tyler.tyler_the_creator++;

		this->on_player_moved(data, tyler, player, Direction::right);
	}

	if (IsKeyPressed(KEY_W))
	{
		player.position.y -= GameData::TILE_SIZE;
		tyler.tyler_the_creator++;
	
		this->on_player_moved(data, tyler, player, Direction::up);
	}

	if (IsKeyPressed(KEY_S))
	{
		player.position.y += GameData::TILE_SIZE;
		tyler.tyler_the_creator++;
	
		this->on_player_moved(data, tyler, player, Direction::down);
	}
}

void Game::on_player_moved(GameData& data, Tyler& tyler, PlayerVarying& player, Direction direction)
{
	Vector2 position = player.position;
	bool hit_corner = false;
	bool hit_box = false;

	Vector2 player_grid = Vector2Scale(position, 1.0f / GameData::TILE_SIZE);

	if (this->m_finished)
	{
		// Reached gate.
		if (player_grid.x == 9 && (player_grid.y == 8 || player_grid.y == 7))
		{
			player.locked = true;
			data.total_moves += tyler.tyler_the_creator;

			data.state_handler.set(GameState::end);
		}
	}

	// Player hits wall
	if (
		this->m_map.map.get_at_position(
			utilities::trunc(player_grid.x), utilities::trunc(player_grid.y),
			SOLID_LAYER
		) == SOLID_WALL
	)
	{
		switch (direction)
		{
		case Direction::left:
			player.position.x += GameData::TILE_SIZE;
			tyler.tyler_the_creator--;

			break;

		case Direction::right:
			player.position.x -= GameData::TILE_SIZE;
			tyler.tyler_the_creator--;

			break;

		case Direction::up:
			player.position.y += GameData::TILE_SIZE;
			tyler.tyler_the_creator--;

			break;

		case Direction::down:
			player.position.y -= GameData::TILE_SIZE;
			tyler.tyler_the_creator--;

			break;
		}

		hit_corner = true;
	}

	if (!this->m_finished)
	{
		this->m_entities.boxes.each([&](Textured&, Position& box) {
			Vector2 box_grid = Vector2Scale(box.position, 1.0f / GameData::TILE_SIZE);

			// Box moves
			if (player_grid == box_grid)
			{
				switch (direction)
				{
				case Direction::left:
					if (this->m_map.map.get_at_position(utilities::trunc(box_grid.x) - 1, utilities::trunc(box_grid.y), SOLID_LAYER) == SOLID_WALL)
					{
						player.position.x += GameData::TILE_SIZE;
						tyler.tyler_the_creator--;

						hit_box = true;
						break;
					}

					box.position.x -= GameData::TILE_SIZE;

					break;

				case Direction::right:
					if (this->m_map.map.get_at_position(utilities::trunc(box_grid.x) + 1, utilities::trunc(box_grid.y), SOLID_LAYER) == SOLID_WALL)
					{
						player.position.x -= GameData::TILE_SIZE;
						tyler.tyler_the_creator--;

						hit_box = true;
						break;
					}

					box.position.x += GameData::TILE_SIZE;

					break;

				case Direction::up:
					if (this->m_map.map.get_at_position(utilities::trunc(box_grid.x), utilities::trunc(box_grid.y) - 1, SOLID_LAYER) == SOLID_WALL)
					{
						player.position.y += GameData::TILE_SIZE;
						tyler.tyler_the_creator--;

						hit_box = true;
						break;
					}

					box.position.y -= GameData::TILE_SIZE;

					break;

				case Direction::down:
					if (this->m_map.map.get_at_position(utilities::trunc(box_grid.x), utilities::trunc(box_grid.y) + 1, SOLID_LAYER) == SOLID_WALL)
					{
						player.position.y -= GameData::TILE_SIZE;
						tyler.tyler_the_creator--;

						hit_box = true;
						break;
					}

					box.position.y += GameData::TILE_SIZE;

					break;
				}
			}

			if (this->m_undos.empty())
			{
				this->m_undos.push_back(
					MoveData(player.position, box.position)
				);
			}
			else
			{
				MoveData last = this->m_undos[this->m_undos.size() - 1];
				if (player.position != last.player_position)
				{
					this->m_undos.push_back(
						MoveData(player.position, box.position)
					);
				}
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

	if (!this->m_undos.empty())
	{
		std::size_t last_idx = this->m_undos.size() - 1;
		MoveData last = this->m_undos[last_idx];

		// todo: these don't make any sense. Just obeying strict rewrites until this is resolved later
		this->m_entities.players.each([x = last.player_position](PlayerShared&, PlayerVarying& player) { player.position = x; });
		this->m_entities.boxes  .each([x = last.box_position   ](Textured    &, Position     & box   ) { box   .position = x; });

		if (this->m_undos.size() > 1)
		{
			this->m_undos.erase(this->m_undos.begin() + last_idx);
		}
	}
}

void Game::awake(GameData& data)
{
	this->m_map = data.maps[data.active_map_index];
	for (int i = 0; i < this->m_map.map.layers.size(); i++)
	{
		std::vector<std::vector<int>> layer = this->m_map.map.layers[i];

		for (int row = 0; row < layer.size(); row++)
		{
			for (int col = 0; col < layer[row].size(); col++)
			{
				if (layer[row][col] == 0) continue;

				const Vector2 position(
					row * this->m_map.map.tile_size.x,
					col * this->m_map.map.tile_size.y);
				switch (layer[row][col])
				{
					case BOX_ID: {
						this->m_entities.boxes.add(position);
						this->m_map.map.set_at_position(row, col, i, 0);
					} break;

					case GOAL_ID: {
						this->m_entities.goals.add(position);
						this->m_map.map.set_at_position(row, col, i, 0);
					} break;

					case PLAYER_ID: {
						this->m_entities.players.add(position);
						this->m_map.map.set_at_position(row, col, i, 0);
					} break;
				}
			}
		}
	}

	this->m_undos.emplace_back();
	this->m_entities.players.each([&m = this->m_undos.back()](PlayerShared&, PlayerVarying& player) { m.player_position = player.position; });
	this->m_entities.boxes  .each([&m = this->m_undos.back()](Textured    &, Position     & box   ) { m.box_position    = box   .position; });

	// Pause UI
	Vector2 resume_dim = this->m_font.measure_text_ex("resume", 10.0f, 0.1f);
	Button resume = Button(
		this->m_font.get(),
		"resume", 10.0f,
		Vector2(
			(GameData::GAME_SIZE.x - resume_dim.x) / 2,
			40
		)
	);

	resume.on_click = [this](Button* /*self*/) {
		this->m_paused = false;
	};

	this->m_buttons.push_back(resume);

	Vector2 menu_dim = this->m_font.measure_text_ex("menu", 10.0f, 0.1f);
	Button menu = Button(
		this->m_font.get(),
		"menu", 10.0f,
		Vector2(
			(GameData::GAME_SIZE.x - menu_dim.x) / 2,
			50
		)
	);

	menu.on_click = [this, &data](Button* /*self*/) {
		data.active_map_index = 0;
		data.total_moves = 0;

		data.state_handler.set(GameState::menu);
	};

	this->m_buttons.push_back(menu);
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
				std::size_t last_idx = undos.size() - 1;
				MoveData last = undos[last_idx];
				if (undos.size() > 1 && player.position == last.player_position)
				{
					undos.erase(undos.begin() + last_idx);
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

	if (this->m_ticks == 30 && !this->m_finished)
	{
		this->m_ticks = 0;

		if (!this->m_switched)
		{
			this->m_entities.visitTylerBoxGoal(
				[this, &data](Tyler& tyler, Position& box, Position& goal) {
					if (
						Vector2Scale(box.position, 1.0f / GameData::TILE_SIZE)
						!=
						Vector2Scale(goal.position, 1.0f / GameData::TILE_SIZE))
					{
						return;
					}

					// The end
					if (data.active_map_index == 11)
					{
						this->m_entities.boxes.varying.clear();

						this->m_switched = true;
						this->m_finished = true;

						// Gate
						this->m_map.map.set_at_position(9, 8, 0, 0);
						this->m_map.map.set_at_position(9, 7, 0, 0);

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

	this->m_entities.goals  .each([](Textured& tex, Position& p) { tex.texture.draw(p.position, WHITE); });
	this->m_entities.boxes  .each([](Textured& tex, Position& p) { tex.texture.draw(p.position, WHITE); });
	this->m_entities.players.each([](Textured& tex, Position& p) { tex.texture.draw(p.position, WHITE); });
	this->m_map.map.draw();

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
			Vector2(
				(GameData::GAME_SIZE.x - dim.x) / 2,
				5
			),
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