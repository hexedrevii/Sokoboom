#pragma once

#include "../../Headers/resource.hpp"

#include <json.hpp>

#include <raylib.h>

#include <filesystem>
#include <vector>

namespace sokoboom {

class Map
{
private:
	Resource::Handle<::Texture2D> m_wall; // todo: cannot be sokoboom::Texture2D because Map is copied
	nlohmann::json m_data;
public:
	Map();
	Map(const std::filesystem::path& path);

	std::vector<std::vector<std::vector<int>>> layers;
	Vector2 tile_size;

	void load(const std::filesystem::path& path);

	void draw();

	int get_at_position(int x, int y, int layer);
	void set_at_position(int x, int y, int layer, int id);
};

} // namespace sokoboom