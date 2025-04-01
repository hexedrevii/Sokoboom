#pragma once

#include "../state.hpp"
#include "../ui.hpp"

#include <array>
#include <format>

namespace sokoboom {

struct GameData;

class End : public State
{
private:
	static constexpr Color clear_color = SKYBLUE;

	std::array<Label, 4> m_labels;

	std::array<Button<End>, 1> m_buttons {{
		{
			"menu", {ui::center, 70},
			[](GameData& data, End& /*end*/, Button<End>& /*self*/) {
				data.play_click();

				data.active_map_index = 0;
				data.total_moves = 0;

				data.transition_state(GameState::menu);
			}
		}
	}};

public:
	explicit End(GameData& data)
		: m_labels {{
			{ "GOOD JOB!", {ui::center, 10} },
			{ "the end!", {ui::center, 22}, 5.0f },
			{ "Hope you enjoyed! <3", {1, 40}, 5.0f },
			{ std::format("{} total moves!", data.total_moves).c_str(), {1, 47}, 5.0f },
		}}
	{
	}

	void awake(GameData& data) override
	{
		reset(*this, data);
	}

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