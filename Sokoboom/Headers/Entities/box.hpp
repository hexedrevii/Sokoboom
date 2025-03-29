#pragma once

#include "entity.hpp"
#include "../resource.hpp"

#include <raylib.h>

#include <filesystem>

namespace sokoboom {

class Box : public Entity 
{
private:
	Texture2D m_texture;

public:
	Box(Vector2 position = Vector2(0, 0))
		: Entity(position)
		, m_texture(resource.texture2d("Content/Entities/box.png"))
	{
	}

	void render() override
	{
		this->m_texture.draw(this->position, WHITE);
	}
};

} // namespace sokoboom