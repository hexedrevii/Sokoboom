#pragma once

#include "../button.hpp"
#include "../data.hpp"
#include "../utilities.hpp"
#include "state.hpp"

#include <raylib.h>
#include <raymath.h>

#include <filesystem>
#include <memory>

namespace sokoboom {

class End : public State
{
private:
	Font m_font = { 0 };
	Sound m_click = { 0 };

	std::vector<Button> m_buttons;
public:
	End()
	{
		this->m_font = utilities::load_font_relative(std::filesystem::path("Content/pico-8.ttf"));
		this->m_click = utilities::load_sound_relative(std::filesystem::path("Content/Audio/click.wav"));
	}

	void awake(GameData& data) override;
	void process(GameData& data) override;
	void render(GameData& data) override;
	void leave() override;
};

inline void End::leave()
{
	UnloadFont(this->m_font);
	UnloadSound(this->m_click);
}

} // namespace sokoboom