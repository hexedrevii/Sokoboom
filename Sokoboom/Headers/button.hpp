#pragma once

#include "../Headers/resource.hpp"

#include <raylib.h>
#include <raymath.h>

#include <string>
#include <functional>

namespace sokoboom {

class Button
{
private:
	Resource::Handle<::Font> m_font;
	std::string m_text;

	Vector2 m_position;
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
		, m_position(pos)
		, m_size(size)
		, on_click(on_click ? on_click : nop)
	{
		Vector2 dim = MeasureTextEx(
			resource[this->m_font], this->m_text.c_str(),
			this->m_size, 0.1f
		);

		this->m_bounds = Rectangle(
			this->m_position.x, this->m_position.y,
			dim.x, dim.y
		);
	}

	~Button()
	{
		resource.release(this->m_font);
	}

	void set_text(std::string new_text)
	{
		this->m_text = std::move(new_text);

		Vector2 dim = MeasureTextEx(
			resource[this->m_font], this->m_text.c_str(),
			this->m_size, 0.1f
		);

		this->m_bounds = Rectangle(
			this->m_position.x, this->m_position.y,
			dim.x, dim.y
		);
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
			resource[this->m_font], this->m_text.c_str(),
			this->m_position, Vector2Zero(),
			0, this->m_size, 0.1f, this->colour
		);
	}
};

} // namespace sokoboom