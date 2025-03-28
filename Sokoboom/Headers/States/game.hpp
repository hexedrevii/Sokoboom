#pragma once

#include "state.hpp"
#include "../map.hpp"
#include "../Entities/player_direction.hpp"
#include "../Entities/player.hpp"
#include "../Entities/goal.hpp"
#include "../Entities/box.hpp"
#include "../data.hpp"
#include "../button.hpp"

#include <raylib.h>

#include <vector>
#include <iostream>
#include <memory>

namespace sokoboom {

struct MoveData
{
	Vector2 player_position;
	Vector2 box_position;
};

class Game : public State
{
private:
	int m_ticks = 0;

	Font m_font = { 0 };

	Sound m_move {};
	Sound m_next {};
	Sound m_explode {};

	MapData m_map;

	EntityController m_entities = EntityController();

	std::weak_ptr<Player> m_player;
	
	std::weak_ptr<Box> m_box;
	std::weak_ptr<Goal> m_goal;

	std::vector<MoveData> m_undos;

	void on_player_moved(GameData& data, Vector2 position, Direction direction);

	bool m_switched = false;

	float m_time = 0;
	float m_undo_delay = 0.35f;
	bool m_undoing = false;
	void undo();

	bool m_finished = false;

	bool m_paused = false;
	std::vector<Button> m_buttons;
public:
	Game(MapData map) : m_map(map) {}

	void awake(GameData& data) override;
	void process(GameData& data) override;
	void render(GameData& data) override;
	void leave() override;
};

} // namespace sokoboom