#pragma once

#include <raylib.h>

#include <bit>
#include <cassert>
#include <cstddef>
#include <utility>

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

template <typename Payload>
constexpr float nan_embed(Payload payload) noexcept
{
	return std::bit_cast<float>(0x7ff0'0000u | static_cast<std::uint32_t>(payload));
}

template <typename Payload>
struct NanExtract
{
	bool is_nan;
	Payload payload;
};

template <typename Payload>
constexpr NanExtract<Payload> nan_extract(float x) noexcept
{
	const auto n = std::bit_cast<std::uint32_t>(x);
	return {
		(n & 0x7ff0'0000u) == 0x7ff0'0000u,
		Payload(n & 0xffffu)
	};
}

constexpr bool is_nan(float x) noexcept
{
	return x != x;
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

template <typename T, typename... Args>
void reset(T& t, Args&&... args)
{
	t.~T();
	new (&t) T(std::forward<Args>(args)...);
}

template <typename T>
union Uninit
{
	char _;
	T data;

	Uninit() : _{} {}
	~Uninit() {}

	template <typename... Args>
	void ctor(Args&&... args) { new (this) T(std::forward<Args>(args)...); }
	void dtor() noexcept { this->data.~T(); }

	constexpr T* operator& () noexcept { return &this->data; }
	constexpr T* operator->() noexcept { return &this->data; }

	constexpr operator T&() noexcept { return this->data; }
};

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