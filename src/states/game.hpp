#pragma once

#include "../data.hpp"
#include "../map.hpp"
#include "../state.hpp"
#include "pause.hpp"

#include <vector>

namespace sokoboom {

#define SOKOBOOM_X_DIRECTIONS(X)\
	X(left , (-1, +0))\
	X(right, (+1, +0))\
	X(up   , (+0, -1))\
	X(down , (+0, +1))

enum class Direction
{
#define X(name, vec) name,
	SOKOBOOM_X_DIRECTIONS(X)
#undef X
};

struct GameData;

class Game : public State
{
private:
	static constexpr auto font = Resource::fixed::Font::pico8;
	static constexpr float undo_repeat_delay_initial = 0.35f;
	static constexpr float undo_repeat_delay         = 0.05f;

	Map m_map;

	struct Position
	{
		Map::Position position;
	};
	
	Position m_goal;
	Position m_box;
	std::size_t m_box_count = 0;

	struct Player : Position
	{
		int tyler_the_creator = 0;
	} m_player;

	struct MoveData
	{
		Map::Position player_position;
		Map::Position box_position;
	};
	std::vector<MoveData> m_undos; // size() >= 1

	void process_player(GameData& data);
	bool move_player(GameData& data, Direction direction);

	float m_undo_time = NAN;
	void record_undo();
	void undo();

	struct Finished : State
	{
		Game* game;

		explicit Finished(Game& game)
			: game(&game)
		{
		}

		void process(GameData& data) override
		{
			game->process_player(data);

			// Reached gate.
			// todo: generalize
			if (game->m_player.position.x == 9
				&& (game->m_player.position.y == 8 || game->m_player.position.y == 7))
			{
				data.total_moves += game->m_player.tyler_the_creator;
				data.transition_state(GameState::end);
			}
		}

		void render(GameData& data) override
		{
			return game->render(data);
		}
	} m_finished { *this };

	Pause m_pause { *this };

public:
	void awake(GameData& data) override;
	void process(GameData& data) override;
	void render(GameData& data) override;
};

} // namespace sokoboom