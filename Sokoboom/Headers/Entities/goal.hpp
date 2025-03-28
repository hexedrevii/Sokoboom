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
	void leave() override;
};

inline Goal::Goal() : Entity(0)
{
	this->m_texture = utilities::load_relative("Content/Entities/goal.png");
}

inline void Goal::render()
{
	DrawTextureV(this->m_texture, this->position, WHITE);
}

inline void Goal::leave()
{
	UnloadTexture(this->m_texture);
}

} // namespace sokoboom