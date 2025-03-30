#include "../../Headers/States/end.hpp"

#include "../../Headers/States/menu.hpp"

#include <format>

namespace sokoboom {

void End::awake(GameData& data)
{
	const Vector2 menu_dim = this->m_font.measure_text_ex("menu", 10.0f, 0.1f);
	this->m_buttons.emplace_back(
		this->m_font.get(),
		"menu", 10.0f,
		Vector2((GameData::GAME_SIZE.x - menu_dim.x) / 2, 70),
		[this, &data](Button& /*self*/) {
			if (!data.mute_sfx) this->m_click();

			data.active_map_index = 0;
			data.total_moves = 0;

			data.state_handler.set(GameState::menu);
		}
	);
}

void End::process(GameData& data)
{
	for (Button& btn : this->m_buttons)
	{
		btn.process(data.virtual_mouse);
	}
}

void End::render(GameData& data)
{
	ClearBackground(SKYBLUE);
	
	Vector2 dim = this->m_font.measure_text_ex("GOOD JOB!", 10.0f, 0.1f);
	this->m_font.draw_text_pro(
		"GOOD JOB!",
		Vector2(
			(GameData::GAME_SIZE.x - dim.x) / 2,
			10
		),
		Vector2Zero(),
		0, 10.0f, 0.1f, WHITE
	);

	Vector2 end_dim = this->m_font.measure_text_ex("the end!", 5.0f, 0.1f);
	this->m_font.draw_text_pro(
		"the end!",
		Vector2(
			(GameData::GAME_SIZE.x - end_dim.x) / 2,
			22
		),
		Vector2Zero(),
		0, 5.0f, 0.1f, WHITE
	);

	this->m_font.draw_text_pro(
		"Hope you enjoyed! <3",
		Vector2(1, 40),
		Vector2Zero(),
		0, 5.0f, 0.1f, WHITE
	);

	this->m_font.draw_text_pro(
		std::format("{} total moves!", data.total_moves).c_str(),
		Vector2(1, 47),
		Vector2Zero(),
		0, 5.0f, 0.1f, WHITE
	);

	for (Button& btn : this->m_buttons)
	{
		btn.render();
	}
}

} // namespace sokoboom