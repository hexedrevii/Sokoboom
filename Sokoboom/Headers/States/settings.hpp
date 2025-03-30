#pragma once

#include "../button.hpp"
#include "../data.hpp"
#include "../resource.hpp"
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

	Sound m_click = resource.sound("Content/Audio/click.wav");
	Font m_font = resource.font("Content/pico-8.ttf");

	// todo: pair with GameData. There's no "commit", so these seem redundant.
	bool m_mute_move = false;
	bool m_mute_sfx = false;

public:
	void awake(GameData& data) override;
	void process(GameData& data) override;
	void render(GameData& data) override;
};

} // namespace sokoboom