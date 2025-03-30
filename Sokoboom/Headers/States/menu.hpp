#pragma once

#include "../button.hpp"
#include "../resource.hpp"
#include "state.hpp"

#include <raylib.h>

#include <vector>

namespace sokoboom {

struct GameData;

class Menu : public State
{
private:
	Font m_font = resource.font("Content/pico-8.ttf");
	Sound m_click = resource.sound("Content/Audio/click.wav");

	std::vector<Button> m_buttons;

public:
	Menu();

	void awake(GameData& data) override;
	void process(GameData& data) override;
	void render(GameData& data) override;
};

} // namespace sokoboom