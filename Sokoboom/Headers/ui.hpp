#pragma once

#include "data.hpp"
#include "resource.hpp"

#include <raylib.h>
#include <raymath.h>

#include <functional>
#include <vector>

namespace sokoboom {

class Button
{
private:
	Font m_font;
	std::string m_text;

	Rectangle m_bounds;

	float m_size = 0;

	static void nop(Button&) {}
	std::function<void(Button&)> on_click = nop;

public:
	Color colour = WHITE;

	Button(
		Resource::Handle<::Font> font,
		std::string text,
		float size,
		Vector2 pos,
		std::function<void(Button&)> on_click)
		: m_font(font)
		, m_text(std::move(text))
		, m_size(size)
		, on_click(on_click ? on_click : nop)
	{
		const Vector2 dim = MeasureTextEx(
			resource[this->m_font.get()], this->m_text.c_str(),
			this->m_size, 0.1f
		);

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

		this->m_bounds.width  = dim.x;
		this->m_bounds.height = dim.y;
	}

	void process(Vector2 mouse)
	{
		if (CheckCollisionPointRec(mouse, this->m_bounds))
		{
			this->colour = YELLOW;
			if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
			{
				this->on_click(*this);
			}
		}
		else
		{
			this->colour = WHITE;
		}
	}

	void render()
	{
		DrawTextPro(
			resource[this->m_font.get()], this->m_text.c_str(),
			Vector2(this->m_bounds.x, this->m_bounds.y), Vector2Zero(),
			0, this->m_size, 0.1f, this->colour
		);
	}
};

} // namespace sokoboom