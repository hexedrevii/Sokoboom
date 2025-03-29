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
	Box();

	void render() override;
};

inline Box::Box()
	: Entity(0)
	, m_texture(resource.texture2d("Content/Entities/box.png"))
{
}

inline void Box::render()
{
	this->m_texture.draw(this->position, WHITE);
}

} // namespace sokoboom