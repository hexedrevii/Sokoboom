#pragma once

#include "../data.hpp"
#include "../state.hpp"
#include "../ui.hpp"

#include <raylib.h>

#include <array>

namespace sokoboom {

class Pause : public State
{
	static constexpr auto fade_color = BLACK;
	static constexpr auto fade_alpha = 0.7f;

	std::array<Label, 1> m_labels {{
		{ "PAUSED", {ui::center, 5} },
	}};

	std::array<Button<Pause>, 2> m_buttons {{
	{
		"resume", {ui::center, 40},
		[](GameData& data, Pause& pause, Button<Pause>& /*self*/) {
			pause.close(data);
		}
	},
	{
		"menu", {ui::center, 50},
		[](GameData& data, Pause& /*pause*/, Button<Pause>& /*self*/) {
			data.active_map_index = 0;
			data.total_moves = 0;

			data.transition_state(GameState::menu);
		}
	}
	}};

	State* m_prev = nullptr; // m_prev != nullptr

	void close(GameData& data)
	{
		data.state_handler.replace(*m_prev);
	}

public:
	explicit Pause(State& state)
		: m_prev(&state)
	{
	}

	void process(GameData& data) override
	{
		if (IsKeyPressed(KEY_ESCAPE))
		{
			this->close(data);
			return;
		}

		for (auto& button : this->m_buttons) button.process(data, *this);
	}

	void render(GameData& data) override
	{
		m_prev->render(data);
		DrawRectangleV({}, GameData::GAME_SIZE, ::Fade(fade_color, fade_alpha));

		for (auto& label  : this->m_labels ) label .render();
		for (auto& button : this->m_buttons) button.render();
	}
};

} // namespace sokoboom