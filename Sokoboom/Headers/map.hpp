#pragma once

#include "../../Headers/resource.hpp"
#include "../../Headers/utilities.hpp"

#include <raylib.h>

#include <filesystem>
#include <vector>

namespace sokoboom {

class Map
{
public:
	using Position = int2;

	enum class CellKind
	{
		none = 0,
		wall = 1,
		box = 2,
		goal = 3,
		player = 4,
	};

	enum class Layer : std::size_t
	{
		solid = 0,
	};

	Map() = default;
	Map(const char* path);

	std::vector<std::vector<std::vector<CellKind>>> layers;
	Vector2 tile_size {}; // todo: conflicts with GameData::TILE_SIZE

	void load(const char* path);

	void draw(Resource::Handle<::Texture2D> wall);

	CellKind get_at_position(Position x                  , Layer       layer) { return this->get_at_position(x.x, x.y, layer             ); }
	CellKind get_at_position(Position x                  , std::size_t layer) { return this->get_at_position(x.x, x.y, layer             ); }
	CellKind get_at_position(std::size_t x, std::size_t y, Layer       layer) { return this->get_at_position(x  , y  , std::size_t(layer)); }
	CellKind get_at_position(std::size_t x, std::size_t y, std::size_t layer) { return this->layers[layer][x][y]; }

	void set_at_position(Position x,                   Layer       layer, CellKind kind) { this->set_at_position(x.x, x.y, layer, kind             ); }
	void set_at_position(Position x,                   std::size_t layer, CellKind kind) { this->set_at_position(x.x, x.y, layer, kind             ); }
	void set_at_position(std::size_t x, std::size_t y, Layer       layer, CellKind kind) { this->set_at_position(x  , y  , std::size_t(layer), kind); }
	void set_at_position(std::size_t x, std::size_t y, std::size_t layer, CellKind kind) { this->layers[layer][x][y] = kind; }
};

} // namespace sokoboom