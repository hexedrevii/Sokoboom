#include "../../Headers/States/settings.hpp"

#include "../../Headers/data.hpp"

#include <json.hpp>

#include <filesystem>
#include <fstream>

namespace sokoboom {

void Settings::awake(GameData& data)
{
	this->m_mute_sfx = data.mute_sfx;
	this->m_mute_move = data.mute_move;

	this->m_buttons.emplace_back(
		resource.getHandle(font),
		"< BACK", 10.0f,
		Vector2(1, GameData::GAME_SIZE.y - 11),
		[this, &data](Button& /*self*/) {
			data.play_click();

			if (std::ofstream f{GetApplicationDirectory() / std::filesystem::path("Content/settings.json")})
			{
				nlohmann::json json;
				json["mute_sfx"] = this->m_mute_sfx;
				json["mute_move"] = this->m_mute_move;

				data.mute_sfx = this->m_mute_sfx;
				data.mute_move = this->m_mute_move;

				f << json.dump(4);
			}

			data.state_handler.set(GameState::menu);
		}
	);

	{
		const auto sfx_text = this->m_mute_sfx ? "UNMUTE SFX" : "MUTE SFX";
		this->m_buttons.emplace_back(
			resource.getHandle(font),
			sfx_text, 5.0f,
			Vector2(1, 30),
			[this, &data](Button& self) {
				data.play_click();

				this->m_mute_sfx = !this->m_mute_sfx;
				self.set_text(this->m_mute_sfx ? "UNMUTE SFX" : "MUTE SFX");
			}
		);
	}

	{
		const auto move_text = this->m_mute_move ? "UNMUTE MOVE" : "MUTE MOVE";
		this->m_buttons.emplace_back(
			resource.getHandle(font),
			move_text, 5.0f,
			Vector2(1, 37),
			[this, &data](Button& self) {
				data.play_click();

				this->m_mute_move = !this->m_mute_move;
				self.set_text(this->m_mute_move ? "UNMUTE MOVE" : "MUTE MOVE");
			}
		);
	}
}

void Settings::process(GameData& data)
{
	for (Button& btn : this->m_buttons)
	{
		btn.process(data.virtual_mouse);
	}
}

void Settings::render(GameData& /*data*/)
{
	ClearBackground(SKYBLUE);

	// Background
	DrawRectangleV(Vector2Zero(), GameData::GAME_SIZE, Fade(BLACK, 0.8f));

	auto& fnt = resource[font];
	Vector2 dim = MeasureTextEx(fnt, "SETTINGS", 10.0f, 0.1f);
	DrawTextPro(
		fnt,
		"SETTINGS",
		Vector2((GameData::GAME_SIZE.x - dim.x) / 2, 7),
		Vector2Zero(),
		0, 10.0f, 0.1f, WHITE
	);

	for (Button& btn : this->m_buttons)
	{
		btn.render();
	}
}

} // namespace sokoboom