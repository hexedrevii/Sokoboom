#include "../Headers/map.hpp"

#include "../Headers/data.hpp"

#include <json.hpp>

#include <raymath.h>

#include <fstream>

namespace sokoboom {

Map::Map(const char* path)
{
	load(path);
}

void Map::load(const char* path)
{
	const auto data = [&path] {
		std::ifstream file(GetApplicationDirectory() / std::filesystem::path(path));
		return nlohmann::json::parse(file);
	}();

	this->layers = data["layers"];
	this->tile_size = Vector2(data["tile_x"], data["tile_y"]);
}

void Map::draw(::Texture2D& wall)
{
	for (const auto& layer : this->layers)
	{
		for (std::size_t row = 0, rowSize = layer.size(); row < rowSize; ++row)
		{
			for (std::size_t col = 0, colSize = layer[row].size(); col < colSize; ++col)
			{
				if (layer[row][col] == CellKind::none) continue;

				DrawTextureV(
					wall,
					Vector2(float(row), float(col)) * GameData::TILE_SIZE,
					WHITE
				);
			}
		}
	}
}

} // namespace sokoboom