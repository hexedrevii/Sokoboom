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
	Font m_font = resource.font("Content/pico-8.ttf");
	Sound m_click = resource.sound("Content/Audio/click.wav");

	std::vector<Button> m_buttons;

public:
	void awake(GameData& data) override;
	void process(GameData& data) override;
	void render(GameData& data) override;
};

} // namespace sokoboom