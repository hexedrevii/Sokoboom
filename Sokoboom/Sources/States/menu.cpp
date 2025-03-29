#include "../../Headers/States/menu.hpp"

#include "../../Headers/utilities.hpp"
#include "../../Headers/States/settings.hpp"
#include "../../Headers/States/game.hpp"

#include <raymath.h>

#include <filesystem>
#include <format>

namespace sokoboom {

Menu::Menu()
	: m_font(resource.font("Content/pico-8.ttf"))
	, m_click(resource.sound("Content/Audio/click.wav"))
{
	SetTextureFilter(resource[this->m_font.get()].texture, TEXTURE_FILTER_POINT); // todo: modified shared resource
}

// Build UI here since `this` will be incomplete in Menu::Menu
void Menu::awake(GameData& data)
{
	Vector2 play_dim = this->m_font.measure_text_ex("play", 10.0f, 0.1f);
	Button play = Button(
		this->m_font.get(),
		"play", 10.0f,
		Vector2(
			(GameData::GAME_SIZE.x - play_dim.x) / 2,
			50
		)
	);

	play.on_click = [this, &data](Button* /*self*/) {
		if (!data.mute_sfx) this->m_click();

		data.state_handler.set(GameState::game);
	};

	this->m_buttons.push_back(play);

	Vector2 options_dim = this->m_font.measure_text_ex("options", 10.0f, 0.1f);
	Button options = Button(
		this->m_font.get(),
		"options", 10.0f,
		Vector2(
			(GameData::GAME_SIZE.x - options_dim.x) / 2,
			60
		)
	);

	options.on_click = [this, &data](Button* /*self*/) {
		if (!data.mute_sfx) this->m_click();

		data.state_handler.set(GameState::settings);
	};

	this->m_buttons.push_back(options);

	Vector2 exit_dim = this->m_font.measure_text_ex("exit", 10.0f, 0.1f);
	Button exit = Button(
		this->m_font.get(),
		"exit", 10.0f,
		Vector2(
			(GameData::GAME_SIZE.x - exit_dim.x) / 2,
			70
		)
	);

	exit.on_click = [this, &data](Button* /*self*/) {
		if (!data.mute_sfx) this->m_click();

		data.exit = true;
	};

	this->m_buttons.push_back(exit);
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

	Vector2 dim = this->m_font.measure_text_ex("SOKOBOOM", 10.0f, 0.1f);
	this->m_font.draw_text_pro(
		"SOKOBOOM",
		Vector2(
			(GameData::GAME_SIZE.x - dim.x) / 2,
			15
		),
		Vector2Zero(),
		0, 10.0f, 0.1f, WHITE
	);

	for (Button& btn : this->m_buttons)
	{
		btn.render();
	}
}

} // namespace sokoboom