#pragma once

#include "../resource.hpp"

#include <raylib.h>

#include <vector>

namespace sokoboom {

enum class Direction
{
	left,
	right,
	up,
	down,
};

template <typename Shared, typename Varying>
struct Group
{
	Shared shared;
	std::vector<Varying> varying;

	template <typename... Args>
	Group(Args&&... args)
		: shared(std::forward<Args>(args)...)
	{
	}

	template <typename... Args>
	void add(Args&&... args)
	{
		varying.emplace_back(std::forward<Args>(args)...);
	}

	template <typename Fn>
	void each(Fn&& fn)
	{
		for (auto& e : varying)
		{
			fn(shared, e);
		}
	}
};

struct Textured
{
	Texture2D texture;
};

struct Position
{
	Vector2 position;

	Position(Vector2 position) : position(position) {}
};

struct Tyler
{
	int tyler_the_creator = 0;
};

struct Lockable
{
	bool locked = false;
};

struct PlayerShared : Textured, Tyler
{
	explicit PlayerShared(Resource::Handle<::Texture2D> handle)
		: Textured(handle)
	{
	}
};

struct PlayerVarying : Lockable, Position
{
	PlayerVarying(Vector2 position) : Position(position) {}
};

struct EntityController
{
	Group<Textured, Position> goals;
	Group<Textured, Position> boxes;
	Group<PlayerShared, PlayerVarying> players;

	EntityController()
		: goals(resource.texture2d("Content/Entities/goal.png"))
		, boxes(resource.texture2d("Content/Entities/box.png"))
		, players(resource.texture2d("Content/Entities/player.png"))
	{
	}

	template <typename Fn>
	void visitTylerBoxGoal(Fn&& fn)
	{
		for (auto& box : boxes.varying)
		{
			for (auto& goal : goals.varying)
			{
				fn(players.shared, box, goal);
			}
		}
	}
};

} // namespace sokoboom