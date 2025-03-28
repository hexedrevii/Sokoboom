#pragma once

#include "../button.hpp"
#include "../data.hpp"
#include "state.hpp"

#include <raylib.h>

#include <vector>
#include <memory>

namespace sokoboom {

class Menu : public State
{
private:
	Font m_font = { 0 };
	Sound m_click = { 0 };

	std::vector<Button> m_buttons;
public:
	Menu();

	void awake(GameData& data) override;
	void process(GameData& data) override;
	void render(GameData& data) override;
	void leave() override;
};

inline void Menu::leave()
{
	UnloadFont(this->m_font);
	UnloadSound(this->m_click);
}

} // namespace sokoboom