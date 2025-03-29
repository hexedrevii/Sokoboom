#pragma once

#include "../resource.hpp"
#include "entity.hpp"

#include <raylib.h>

namespace sokoboom {

class Box : public Entity 
{
private:
	Texture2D m_texture;

	friend class EntityController;
	Box(Vector2 position = Vector2(0, 0))
		: Entity(position)
		, m_texture(resource.texture2d("Content/Entities/box.png"))
	{
	}

public:
	void render() override
	{
		this->m_texture.draw(this->position, WHITE);
	}
};

} // namespace sokoboom