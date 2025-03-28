#pragma once

#include "map.hpp"
#include "Controllers/state_controller.hpp"

#include <raylib.h>

#include <memory>

namespace sokoboom {

struct MapData
{
	std::string name;
	Map map;
};

struct GameData
{
	static constexpr int GAP = 7;
	static constexpr int TILE_SIZE = 8;
	static constexpr int MAP_SIZE = 10;

	static constexpr Vector2 GAME_SIZE = Vector2(TILE_SIZE * MAP_SIZE, TILE_SIZE * MAP_SIZE + GAP);

	bool mute_move = false;
	bool mute_sfx = false;
	bool exit = false;

	int total_moves = 0;
	int active_map_index = 0;
	std::vector<MapData> maps;

	StateController state_handler;
	Vector2 virtual_mouse {};
};

} // namespace sokoboom