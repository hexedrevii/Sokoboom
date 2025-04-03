#include "map.hpp"

#include "data.hpp"

#include <json.hpp>

#include <raymath.h>

#include <fstream>
#include <iostream>

namespace sokoboom {

Map::Map(const Map& rhs)
{
	append(rhs);
}

Map::Map(std::string_view path)
{
	load(path);
}

Map& Map::operator=(const Map& rhs)
{
	layers.clear();
	append(rhs);
	return *this;
}

void Map::append(const Map& rhs)
{
	layers.reserve(rhs.layers.size());
	for (const auto& layer : rhs.layers)
	{
		const auto size = offsetof(Grid, base[layer->width * layer->height]);
		layers.emplace_back(static_cast<Grid*>(std::malloc(size)));
		std::memcpy(layers.back().get(), layer.get(), size);
	}
}

enum class CellKind
{
	none = 0,
	wall = 1,
	box = 2,
	goal = 3,
	player = 4,
};

static Map::Cell kindToCell(CellKind kind)
{
	switch (kind)
	{
	case CellKind::none  : return Map::Cell{ .object = Map::Cell::Object::none  , .goal = 0, };
	case CellKind::wall  : return Map::Cell{ .object = Map::Cell::Object::wall  , .goal = 0, };
	case CellKind::box   : return Map::Cell{ .object = Map::Cell::Object::box   , .goal = 0, };
	case CellKind::goal  : return Map::Cell{ .object = Map::Cell::Object::none  , .goal = 1, };
	case CellKind::player: return Map::Cell{ .object = Map::Cell::Object::player, .goal = 0, };
	}
	unreachable();
}

void Map::load(std::string_view path)
{
	const auto data = [&path] {
		std::ifstream file(GetApplicationDirectory() / std::filesystem::path(path));
		return nlohmann::json::parse(file, nullptr, false);
	}();

	const auto& data_layers = data["layers"];
	this->layers.reserve(data_layers.size());
	for (auto& layer : data_layers)
	{
		const std::size_t width = layer.size();
		if (!width)
		{
			std::cerr << "Map " << path << " has invalid width: " << width << '\n';
			goto L_error;
		}

		const std::size_t height = layer.front().size();
		if (!height)
		{
			std::cerr << "Map " << path << " has invalid height: " << height << '\n';
			goto L_error;
		}

		const std::size_t n = width*height;
		Map::GridOwner grid { static_cast<Grid*>(std::malloc(offsetof(Grid, base[n]))) };
		grid->width = width;
		grid->height = height;
		for (Position pos{}; pos.x < width; ++pos.x)
		{
			for (pos.y = 0; pos.y < height; ++pos.y)
			{
				grid->set(pos, kindToCell(layer[pos.x][pos.y]));
			}
		}
		this->layers.emplace_back(std::move(grid));
	}

	return;

L_error:
	this->layers.clear();
}

void Map::draw(::Texture2D& wall)
{
	for (const auto& layer : this->layers)
	{
		for (Position pos{}; pos.x < layer->width; ++pos.x)
		{
			for (pos.y = 0; pos.y < layer->height; ++pos.y)
			{
				if (!layer->get(pos).is_wall()) continue;

				DrawTextureV(
					wall,
					Vector2(float(pos.x), float(pos.y)) * GameData::TILE_SIZE,
					WHITE
				);
			}
		}
	}
}

} // namespace sokoboom