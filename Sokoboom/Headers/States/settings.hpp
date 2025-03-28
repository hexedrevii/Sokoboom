#pragma once

#include "../button.hpp"
#include "../data.hpp"
#include "../utilities.hpp"
#include "state.hpp"

#include <json.hpp>

#include <raylib.h>
#include <raymath.h>

#include <filesystem>
#include <memory>
#include <vector>

namespace sokoboom {

class Settings : public State
{
private:
	std::vector<Button> m_buttons;

	Sound m_click = { 0 };
	Font m_font = { 0 };

	bool m_mute_move = false;
	bool m_mute_sfx = false;
public:
	Settings()
	{
		this->m_font = utilities::load_font_relative(std::filesystem::path("Content/pico-8.ttf"));
		this->m_click = utilities::load_sound_relative(std::filesystem::path("Content/Audio/click.wav"));
	};

	void awake(GameData& data) override;
	void process(GameData& data) override;
	void render(GameData& data) override;
	void leave() override;
};

inline void Settings::leave()
{
	UnloadFont(this->m_font);
	UnloadSound(this->m_click);
}

} // namespace sokoboom