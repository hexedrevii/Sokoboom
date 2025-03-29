#pragma once

#include <json.hpp>

#include <raylib.h>

#include <filesystem>
#include <map>
#include <vector>

namespace sokoboom {

class Map
{
private:
	std::map<int, Texture2D> m_tiles;
	nlohmann::json m_data;
public:
	Map() = default;
	Map(const std::filesystem::path& path) { load(path); }

	std::vector<std::vector<std::vector<int>>> layers;
	Vector2 tile_size;

	void load(const std::filesystem::path& path);

	void draw();
	void leave();

	int get_at_position(int x, int y, int layer);
	void set_at_position(int x, int y, int layer, int id);
};

} // namespace sokoboom