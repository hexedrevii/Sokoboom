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
	Goal();

	void render() override;
};

inline Goal::Goal()
	: Entity(0)
	, m_texture(resource.texture2d("Content/Entities/goal.png"))
{
}

inline void Goal::render()
{
	this->m_texture.draw(this->position, WHITE);
}

} // namespace sokoboom