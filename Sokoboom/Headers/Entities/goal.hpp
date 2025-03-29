#pragma once

#include "../utilities.hpp"
#include "entity.hpp"

#include <raylib.h>

#include <filesystem>

namespace sokoboom {

class Goal : public Entity 
{
private:
	Texture2D m_texture;
public:
	Goal(int id, Vector2 position = Vector2(0, 0), Vector2 velocity = Vector2(0, 0))
		: Entity(id, position, velocity)
		, m_texture(resource.texture2d("Content/Entities/goal.png"))
	{
	}

	void render() override
	{
		this->m_texture.draw(this->position, WHITE);
	}
};

} // namespace sokoboom