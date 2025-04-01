#pragma once

#include "../resource.hpp"
#include "../ui.hpp"
#include "state.hpp"

#include <array>

namespace sokoboom {

struct GameData;

class Menu : public State
{
private:
	static constexpr Color clear_color = SKYBLUE;

	std::array<Label, 1> m_labels {{
		{ "SOKOBOOM", {ui::center, 15} }
	}};

	std::array<Button<Menu>, 3> m_buttons {{
		{
			"play", {ui::center, 50},
			[](GameData& data, Menu& /*menu*/, Button<Menu>& /*self*/) {
				data.play_click();
				data.change_state(GameState::game);
			}
		},
		{
			"options", {ui::center, 60},
			[](GameData& data, Menu& /*menu*/, Button<Menu>& /*self*/) {
				data.play_click();
				data.change_state(GameState::settings);
			}
		},
		{
			"exit", {ui::center, 70},
			[](GameData& data, Menu& /*menu*/, Button<Menu>& /*self*/) {
				data.play_click();
				data.exit = true;
			}
		}
	}};

public:
	void process(GameData& data) override
	{
		for (auto& button : this->m_buttons) button.process(data, *this);
	}

	void render(GameData& /*data*/) override
	{
		ClearBackground(clear_color);
		for (auto& label  : this->m_labels ) label .render();
		for (auto& button : this->m_buttons) button.render();
	}
};

} // namespace sokoboom