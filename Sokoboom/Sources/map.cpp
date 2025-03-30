#include "../Headers/map.hpp"
#include "../Headers/utilities.hpp"

#include <json.hpp>

#include <fstream>

namespace sokoboom {

Map::Map(std::filesystem::path path)
{
	load(std::move(path));
}

void Map::load(std::filesystem::path path)
{
	const auto data = [&path] {
		std::ifstream file(GetApplicationDirectory() / std::move(path));
		return nlohmann::json::parse(file);
	}();

	this->layers = data["layers"];
	this->tile_size = Vector2(data["tile_x"], data["tile_y"]);
}

void Map::draw(Resource::Handle<::Texture2D> wall)
{
	for (const auto& layer : this->layers)
	{
		for (std::size_t row = 0, rowSize = layer.size(); row < rowSize; ++row)
		{
			for (std::size_t col = 0, colSize = layer[row].size(); col < colSize; ++col)
			{
				if (layer[row][col] == CellKind::none) continue;

				DrawTexture(
					resource[wall],
					utilities::trunc(row * this->tile_size.x), utilities::trunc(col * this->tile_size.y),
					WHITE
				);
			}
		}
	}
}

} // namespace sokoboom