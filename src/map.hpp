#pragma once

#include "resource.hpp"
#include "utilities.hpp"

#include <raylib.h>

#include <memory>
#include <string_view>
#include <vector>

namespace sokoboom {

class Map
{
public:
	using Position = int2;

	struct Cell
	{
		using Type = unsigned char;

		enum class Object : Type
		{
			none,
			wall,
			box,
			player,
		};

		Object object : 2;
		Type   goal   : 1;

		constexpr bool is_none() const noexcept { return this->object == Object::none; }
		constexpr bool is_wall() const noexcept { return this->object == Object::wall; }
	};

	struct Grid
	{
		std::size_t width;
		std::size_t height;

		// Flexible array member is not standard
#		pragma warning(suppress : 4200)
		Cell base[];

		Grid(const Grid& rhs) noexcept
		{
			std::memcpy(this, &rhs, offsetof(Grid, base[rhs.width*rhs.height]));
		}
		Grid& operator=(const Grid& rhs) noexcept
		{
			std::memcpy(this, &rhs, offsetof(Grid, base[rhs.width*rhs.height]));
			return *this;
		}

		Cell operator[](Position p) const noexcept { return this->base[p.x*height + p.y]; }
		Cell get(Position p) const noexcept { return this->operator[](p); }
		Cell get(int x, int y) const noexcept { return this->operator[](Position{x, y}); }

		void set(Position p, Cell cell) noexcept { this->base[p.x*height + p.y] = cell; }
	};
	static_assert(std::is_standard_layout_v<Grid>);
	static_assert(std::is_trivially_destructible_v<Cell>);
	static_assert(std::is_trivially_destructible_v<Grid>);

	enum class Layer : std::size_t
	{
		solid = 0,
	};

	struct GridDeleter
	{
		void operator()(Grid* p)
		{
			std::free(p);
		}
	};
	using GridOwner = std::unique_ptr<Grid, GridDeleter>;
	std::vector<GridOwner> layers;

	Map() = default;
	explicit Map(std::string_view path);

	Map(const Map& rhs);
	Map& operator=(const Map& rhs);

	void append(const Map& rhs);
	void load(std::string_view path);

	void draw(::Texture2D& wall);

	Cell get(Position x, Layer       layer) const { return this->get(x, std::size_t(layer)); }
	Cell get(Position x, std::size_t layer) const { return this->layers[layer]->get(x); }

	void set(Position x, Layer       layer, Cell cell) { this->set(x, std::size_t(layer), cell); }
	void set(Position x, std::size_t layer, Cell cell) { this->layers[layer]->set(x, cell); }
};

} // namespace sokoboom