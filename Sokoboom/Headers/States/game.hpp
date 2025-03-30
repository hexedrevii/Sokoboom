#pragma once

#include "../button.hpp"
#include "../data.hpp"
#include "state.hpp"

#include <raylib.h>

#include <vector>

namespace sokoboom {

struct MoveData
{
	Map::Position player_position;
	Map::Position box_position;
};

class Game : public State
{
private:
	int m_ticks = 0;

	Font m_font = resource.font("Content/pico-8.ttf");

	Sound m_move = resource.sound("Content/Audio/move.wav");
	Sound m_next = resource.sound("Content/Audio/next.wav");
	Sound m_explode = resource.sound("Content/Audio/explosion.wav");

	Texture2D m_wall = resource.texture2d("Content/Props/wall.png");
	MapData m_map;

	struct Object : Textured, Position
	{
		Object(Resource::Handle<::Texture2D> tex, Map::Position pos = {})
			: Textured(tex)
			, Position(pos)
		{
		}
	};

	Object m_goal{resource.texture2d("Content/Entities/goal.png")};
	Object m_box{resource.texture2d("Content/Entities/box.png")};
	std::size_t m_box_count = 0;
	
	struct Player : Object, Tyler, Lockable
	{
		using Object::Object;
	} m_player {resource.texture2d("Content/Entities/player.png")};

	std::vector<MoveData> m_undos; // size() >= 1

	void process_player(GameData& data);
	void move_player(GameData& data, Direction direction);

	bool m_switched = false;

	float m_time = 0;
	float m_undo_delay = 0.35f;
	bool m_undoing = false;
	void undo();

	bool m_finished = false;

	bool m_paused = false;
	std::vector<Button> m_buttons;

public:
	void awake(GameData& data) override;
	void process(GameData& data) override;
	void render(GameData& data) override;
};

} // namespace sokoboom