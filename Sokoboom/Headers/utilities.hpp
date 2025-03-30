#pragma once

#include <cassert>
#include <cstddef>

namespace sokoboom {

constexpr int trunc(float x) noexcept
{
	return int(x);
}

constexpr int shrink(std::size_t x) noexcept
{
	int ret = int(x);
	assert(std::size_t(ret) == x);
	return ret;
}

// todo: replace with std::unreachable()
[[noreturn]]
constexpr void unreachable() noexcept
{
#ifdef _MSC_VER
	__assume(false);
#else
#	error "unreachable not implemented"
#endif
}

template <typename T>
struct V2
{
	T x {};
	T y {};

	constexpr V2() noexcept = default;
	constexpr V2(T x, T y) noexcept
		: x(x)
		, y(y)
	{
	}

	constexpr bool operator==(const V2& rhs) const = default;

	explicit constexpr operator ::Vector2() const
	{
		return ::Vector2{float(x), float(y)};
	}

	constexpr V2 operator-() const noexcept
	{
		return {-this->x, -this->y };
	}

	constexpr V2& operator+=(V2 rhs) noexcept
	{
		this->x += rhs.x;
		this->y += rhs.y;
		return *this;
	}

	friend constexpr V2 operator+(V2 lhs, V2 rhs) noexcept
	{
		return lhs += rhs;
	}

	constexpr V2& operator-=(V2 rhs) noexcept
	{
		this->x -= rhs.x;
		this->y -= rhs.y;
		return *this;
	}

	friend constexpr V2 operator-(V2 lhs, V2 rhs) noexcept
	{
		return lhs -= rhs;
	}

	constexpr V2& operator*=(T rhs) noexcept
	{
		this->x *= rhs;
		this->y *= rhs;
		return *this;
	}

	friend constexpr V2 operator*(V2 lhs, T rhs) noexcept
	{
		return lhs *= rhs;
	}

	constexpr V2& operator/=(T rhs) noexcept
	{
		this->x /= rhs;
		this->y /= rhs;
		return *this;
	}

	friend constexpr V2& operator/(V2 lhs, T rhs) noexcept
	{
		return lhs /= rhs;
	}
};

using int2 = V2<int>;

} // namespace sokoboom