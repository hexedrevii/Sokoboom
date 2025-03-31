#pragma once

#include "map.hpp"
#include "state_controller.hpp"

#include <raylib.h>

namespace sokoboom {

struct MapData
{
	std::string name;
	Map map;
};

struct GameData
{
	static constexpr int GAP = 7;
	static constexpr int TILE_SIZE = 8; // todo: conflicts with Map::tile_size
	static constexpr int MAP_SIZE = 10;

	// todo: conflicts with Map's variable map size
	static constexpr Vector2 GAME_SIZE = Vector2(TILE_SIZE * MAP_SIZE, TILE_SIZE * MAP_SIZE + GAP);

	bool mute_move = false;
	bool mute_sfx = false;
	bool exit = false;

	int total_moves = 0;
	int active_map_index = 0;
	std::vector<MapData> maps;

	StateController state_handler;
	Vector2 virtual_mouse {};

	void play_move   () { if (!this->mute_sfx && !this->mute_move) PlaySound(resource[Resource::fixed::Sound::move]); }
	void play_next   () { if (!this->mute_sfx) PlaySound(resource[Resource::fixed::Sound::next   ]); }
	void play_explode() { if (!this->mute_sfx) PlaySound(resource[Resource::fixed::Sound::explode]); }
	void play_click  () { if (!this->mute_sfx) PlaySound(resource[Resource::fixed::Sound::click  ]); }

	void change_state(GameState state) { state_handler.set(*this, state); }
};

} // namespace sokoboom