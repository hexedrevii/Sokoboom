#include "../../Headers/States/menu.hpp"

#include "../../Headers/utilities.hpp"
#include "../../Headers/States/settings.hpp"
#include "../../Headers/States/game.hpp"

#include <raymath.h>

#include <filesystem>
#include <format>

namespace sokoboom {

Menu::Menu(GameData& data)
{
	SetTextureFilter(resource[font].texture, TEXTURE_FILTER_POINT); // todo: modified shared resource

	auto& fnt = resource[font];
	{
		const Vector2 play_dim = MeasureTextEx(fnt, "play", 10.0f, 0.1f);
		this->m_buttons.emplace_back(
			resource.getHandle(font),
			"play", 10.0f,
			Vector2((GameData::GAME_SIZE.x - play_dim.x) / 2, 50),
			[this, &data](Button& /*self*/) {
				data.play_click();

				data.change_state(GameState::game);
			}
		);
	}

	{
		const Vector2 options_dim = MeasureTextEx(fnt, "options", 10.0f, 0.1f);
		this->m_buttons.emplace_back(
			resource.getHandle(font),
			"options", 10.0f,
			Vector2((GameData::GAME_SIZE.x - options_dim.x) / 2, 60),
			[this, &data](Button& /*self*/) {
				data.play_click();

				data.change_state(GameState::settings);
			}
		);
	}

	{
		const Vector2 exit_dim = MeasureTextEx(fnt, "exit", 10.0f, 0.1f);
		this->m_buttons.emplace_back(
			resource.getHandle(font),
			"exit", 10.0f,
			Vector2((GameData::GAME_SIZE.x - exit_dim.x) / 2, 70),
			[this, &data](Button& /*self*/) {
				data.play_click();

				data.exit = true;
			}
		);
	}
}

void Menu::process(GameData& data)
{
	for (Button& btn : this->m_buttons)
	{
		btn.process(data.virtual_mouse);
	}
}

void Menu::render(GameData& /*data*/)
{
	ClearBackground(SKYBLUE);

	auto& fnt = resource[font];
	Vector2 dim = MeasureTextEx(fnt, "SOKOBOOM", 10.0f, 0.1f);
	DrawTextPro(
		fnt,
		"SOKOBOOM",
		Vector2((GameData::GAME_SIZE.x - dim.x) / 2, 15),
		Vector2Zero(),
		0, 10.0f, 0.1f, WHITE
	);

	for (Button& btn : this->m_buttons)
	{
		btn.render();
	}
}

} // namespace sokoboom