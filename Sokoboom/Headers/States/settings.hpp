#pragma once

#include "../data.hpp"
#include "../state.hpp"
#include "../ui.hpp"

#include <json.hpp>

#include <raylib.h>

#include <array>
#include <filesystem>
#include <fstream>

namespace sokoboom {

class Settings : public State
{
private:
	static constexpr Color clear_color = SKYBLUE;
	static constexpr Color fade_color = BLACK;
	static constexpr float fade_alpha = 0.8f;

	// todo: this only works because these fields precede the m_buttons, so they construct first.
	// The m_buttons depend on the values of these when initializing the button text. This could
	// be resolved in awake(), but awake currently uses to ctor as all other States do so far by
	// convention. This is arguably a bad solution because the dependency isn't very clear and 
	// means moving the order of these fields can introduce a bug. On the other hand initializing
	// the state in awake() duplicates all the initialization code and requires giving names to 
	// the buttons. Play around and see what seems like the best solution.
	bool m_mute_move = false;
	bool m_mute_sfx = false;

	std::array<Label, 1> m_labels {{
		{ "SETTINGS", {ui::center, 7} }
	}};

	std::array<Button<Settings>, 3> m_buttons {{
	{
		"< BACK", {1, GameData::GAME_SIZE.y - 11},
		[](GameData& data, Settings& settings, Button<Settings>& /*self*/) {
			data.play_click();

			data.mute_sfx = settings.m_mute_sfx;
			data.mute_move = settings.m_mute_move;
			data.save_settings();

			data.transition_state(GameState::menu);
		}
	},
	{
		this->m_mute_sfx ? "UNMUTE SFX" : "MUTE SFX", {1, 30}, 5.0f,
		[](GameData& data, Settings& settings, Button<Settings>& self) {
			data.play_click();

			settings.m_mute_sfx = !settings.m_mute_sfx;
			self.set_text(settings.m_mute_sfx ? "UNMUTE SFX" : "MUTE SFX");
		}
	},
	{
		this->m_mute_move ? "UNMUTE MOVE" : "MUTE MOVE", {1, 37}, 5.0f,
		[](GameData& data, Settings& settings, Button<Settings>& self) {
			data.play_click();

			settings.m_mute_move = !settings.m_mute_move;
			self.set_text(settings.m_mute_move ? "UNMUTE MOVE" : "MUTE MOVE");
		}
	}
	}};

public:
	explicit Settings(GameData& data)
		: m_mute_sfx(data.mute_sfx)
		, m_mute_move(data.mute_move)
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
		DrawRectangleV({}, GameData::GAME_SIZE, ::Fade(fade_color, fade_alpha));

		for (auto& label  : this->m_labels ) label .render();
		for (auto& button : this->m_buttons) button.render();
	}
};

} // namespace sokoboom