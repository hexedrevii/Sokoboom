#pragma once

#include "../data.hpp"
#include "../entity.hpp"
#include "../state.hpp"
#include "pause.hpp"

#include <vector>

namespace sokoboom {

struct GameData;

struct MoveData
{
	Map::Position player_position;
	Map::Position box_position;
};

class Game : public State
{
private:
	static constexpr auto font = Resource::fixed::Font::pico8;

	int m_ticks = 0;

	Map m_map;

	Position m_goal;
	Position m_box;
	std::size_t m_box_count = 0;
	
	struct Player : Position, Tyler, Lockable
	{
	} m_player;

	std::vector<MoveData> m_undos; // size() >= 1

	void process_player(GameData& data);
	void move_player(GameData& data, Direction direction);

	bool m_switched = false;

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