#pragma once

#include "data.hpp"
#include "resource.hpp"
#include "utilities.hpp"

#include <raylib.h>

#include <cstdint>

namespace sokoboom {

namespace ui {
	enum class PositionCode : std::uint32_t
	{
		center = 1,
	};

	constexpr float center = nan_embed(PositionCode::center);
}

class Label
{
public:
	static constexpr auto default_font = Resource::fixed::Font::pico8;
	static constexpr auto default_size = 10.0f;
	static constexpr auto default_spacing = 0.1f;
	static constexpr auto default_color = WHITE;

protected:
	Font m_font;
	std::string m_text;
	Vector2 m_pos;
	Rectangle m_bounds;
	float m_size = default_size;
	float m_spacing = default_spacing;
	Color m_colour = default_color;

public:
	Label(std::string text, Vector2 pos, float size = default_size)
		: Label(
			resource.handle(default_font),
			std::move(text),
			pos,
			size)
	{
	}

	Label(
		Font font,
		std::string text,
		Vector2 pos,
		float size)
		: m_font(std::move(font))
		, m_text(std::move(text))
		, m_pos(pos)
		, m_size(size)
	{
		const Vector2 dim = MeasureTextEx(
			resource[this->m_font.get()], this->m_text.c_str(),
			this->m_size, this->m_spacing
		);

		const auto [nan, code] = nan_extract<ui::PositionCode>(pos.x);
		if (nan)
		{
			switch (code)
			{
			case ui::PositionCode::center: pos.x = (GameData::GAME_SIZE.x - dim.x) / 2; break;
			default: unreachable();
			}
		}

		this->m_bounds = Rectangle(
			pos.x, pos.y,
			dim.x, dim.y
		);
	}

	void set_text(std::string new_text)
	{
		this->m_text = std::move(new_text);

		const Vector2 dim = MeasureTextEx(
			resource[this->m_font.get()], this->m_text.c_str(),
			this->m_size, 0.1f
		);

		const auto [nan, code] = nan_extract<ui::PositionCode>(this->m_pos.x);
		if (nan)
		{
			switch (code)
			{
			case ui::PositionCode::center: m_bounds.x = (GameData::GAME_SIZE.x - dim.x) / 2; break;
			default: unreachable();
			}
		}

		this->m_bounds.width  = dim.x;
		this->m_bounds.height = dim.y;
	}

	void render()
	{
		DrawTextPro(
			resource[this->m_font.get()], this->m_text.c_str(),
			Vector2(this->m_bounds.x, this->m_bounds.y), {},
			0, this->m_size, this->m_spacing, this->m_colour
		);
	}
};

template <typename ParentState>
class Button : public Label
{
public:
	using Action = void (*)(GameData&, ParentState&, Button&);

private:
	static constexpr auto color_hover  = YELLOW;

	static void nop(GameData&, ParentState&, Button&) {}
	Action m_on_click = nop;

public:
	Button(std::string text, Vector2 pos, Action on_click)
		: Button(
			resource.handle(default_font),
			std::move(text),
			pos,
			default_size,
			on_click)
	{
	}

	Button(std::string text, Vector2 pos, float size, Action on_click)
		: Button(
			resource.handle(default_font),
			std::move(text),
			pos,
			size,
			on_click)
	{
	}

	Button(
		Font font,
		std::string text,
		Vector2 pos,
		float size,
		Action on_click)
		: Label(std::move(font), std::move(text), pos, size)
		, m_on_click(on_click ? on_click : nop)
	{
	}

	void process(GameData& data, ParentState& state)
	{
		if (CheckCollisionPointRec(data.virtual_mouse, this->m_bounds))
		{
			this->m_colour = color_hover;
			if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
			{
				this->m_on_click(data, state, *this);
			}
		}
		else
		{
			this->m_colour = default_color;
		}
	}
};

} // namespace sokoboom