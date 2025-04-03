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

	int m_ticks = 0;

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
	void move_player(GameData& data, Direction direction);

	float m_time = 0;
	float m_undo_delay = 0.35f;
	bool m_undoing = false;
	void undo();

	bool m_finished = false;

	Pause m_pause { *this };

public:
	void awake(GameData& data) override;
	void process(GameData& data) override;
	void render(GameData& data) override;
};

} // namespace sokoboom