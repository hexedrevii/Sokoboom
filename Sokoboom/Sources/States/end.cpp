#include "../../Headers/States/end.hpp"

#include "../../Headers/data.hpp"

#include <format>

namespace sokoboom {

End::End(GameData& data)
{
	const Vector2 menu_dim = MeasureTextEx(resource[font], "menu", 10.0f, 0.1f);
	this->m_buttons.emplace_back(
		resource.handle(font),
		"menu", 10.0f,
		Vector2((GameData::GAME_SIZE.x - menu_dim.x) / 2, 70),
		[this, &data](Button& /*self*/) {
			data.play_click();

			data.active_map_index = 0;
			data.total_moves = 0;

			data.change_state(GameState::menu);
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
	
	auto& fnt = resource[font];
	Vector2 dim = MeasureTextEx(fnt, "GOOD JOB!", 10.0f, 0.1f);
	DrawTextPro(
		fnt,
		"GOOD JOB!",
		Vector2((GameData::GAME_SIZE.x - dim.x) / 2, 10),
		Vector2Zero(),
		0, 10.0f, 0.1f, WHITE
	);

	Vector2 end_dim = MeasureTextEx(fnt, "the end!", 5.0f, 0.1f);
	DrawTextPro(
		fnt,
		"the end!",
		Vector2((GameData::GAME_SIZE.x - end_dim.x) / 2, 22),
		Vector2Zero(),
		0, 5.0f, 0.1f, WHITE
	);

	DrawTextPro(
		fnt,
		"Hope you enjoyed! <3",
		Vector2(1, 40),
		Vector2Zero(),
		0, 5.0f, 0.1f, WHITE
	);

	DrawTextPro(
		fnt,
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