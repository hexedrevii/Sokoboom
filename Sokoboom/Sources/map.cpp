#include "../Headers/map.hpp"
#include "../Headers/utilities.hpp"

#include <fstream>
#include <stdexcept>

namespace sokoboom {

Map::Map()
	: m_wall(resource.texture2d("Content/Props/wall.png"))
{
}

Map::Map(const std::filesystem::path& path)
	: m_wall(resource.texture2d("Content/Props/wall.png"))
{
	load(path);
}

void Map::load(const std::filesystem::path& path)
{
	std::filesystem::path full = GetApplicationDirectory() / path;
	std::ifstream file(full);
	this->m_data = nlohmann::json::parse(file);
	file.close();

	this->layers = this->m_data["layers"];
	this->tile_size = Vector2(this->m_data["tile_x"], this->m_data["tile_y"]);
}

void Map::draw()
{
	for (const std::vector<std::vector<int>>& layer : this->layers)
	{
		for (size_t row = 0; row < layer.size(); row++)
		{
			for (size_t col = 0; col < layer[row].size(); col++)
			{
				if (layer[row][col] == 0) continue;

				DrawTexture(
					resource[this->m_wall],
					utilities::trunc(row * this->tile_size.x), utilities::trunc(col * this->tile_size.y),
					WHITE
				);
			}
		}
	}
}

int Map::get_at_position(int x, int y, int layer)
{
	return this->layers[layer][x][y];
}

void Map::set_at_position(int x, int y, int layer, int id)
{
	this->layers[layer][x][y] = id;
}

} // namespace sokoboom