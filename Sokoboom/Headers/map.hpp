#pragma once

#include "../../Headers/resource.hpp"

#include <raylib.h>

#include <filesystem>
#include <vector>

namespace sokoboom {

struct TilePosition
{
	std::size_t x = 0;
	std::size_t y = 0;

	TilePosition() = default;
	TilePosition(std::size_t x, std::size_t y) : x(x), y(y) {}

	constexpr bool operator==(const TilePosition& rhs) const = default;

	explicit operator Vector2() const
	{
		return Vector2{float(x), float(y)};
	}

	TilePosition& operator+=(TilePosition rhs)
	{
		this->x += rhs.x;
		this->y += rhs.y;
		return *this;
	}

	friend TilePosition operator+(TilePosition lhs, TilePosition rhs)
	{
		return lhs += rhs;
	}

	TilePosition& operator-=(TilePosition rhs)
	{
		this->x -= rhs.x;
		this->y -= rhs.y;
		return *this;
	}

	friend TilePosition operator-(TilePosition lhs, TilePosition rhs)
	{
		return lhs -= rhs;
	}

	TilePosition& operator*=(std::size_t rhs)
	{
		this->x *= rhs;
		this->y *= rhs;
		return *this;
	}

	friend TilePosition operator*(TilePosition lhs, std::size_t rhs)
	{
		return lhs *= rhs;
	}
};

class Map
{
public:
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
	Map(std::filesystem::path path);

	std::vector<std::vector<std::vector<CellKind>>> layers;
	Vector2 tile_size {}; // todo: conflicts with GameData::TILE_SIZE

	void load(std::filesystem::path path);

	void draw(Resource::Handle<::Texture2D> wall);

	CellKind get_at_position(TilePosition x, Layer layer)
	{
		return this->get_at_position(x.x, x.y, layer);
	}

	CellKind get_at_position(std::size_t x, std::size_t y, Layer layer)
	{
		return this->get_at_position(x, y, std::size_t(layer));
	}

	CellKind get_at_position(std::size_t x, std::size_t y, std::size_t layer)
	{
		return this->layers[layer][x][y];
	}

	void set_at_position(TilePosition x, Layer layer, CellKind kind)
	{
		this->set_at_position(x.x, x.y, layer, kind);
	}

	void set_at_position(TilePosition x, std::size_t layer, CellKind kind)
	{
		this->set_at_position(x.x, x.y, layer, kind);
	}

	void set_at_position(std::size_t x, std::size_t y, Layer layer, CellKind kind)
	{
		this->set_at_position(x, y, std::size_t(layer), kind);
	}

	void set_at_position(std::size_t x, std::size_t y, std::size_t layer, CellKind kind)
	{
		this->layers[layer][x][y] = kind;
	}
};

} // namespace sokoboom