#pragma once

#include "../button.hpp"
#include "state.hpp"

#include <raylib.h>

#include <vector>

namespace sokoboom {

struct GameData;

class Menu : public State
{
private:
	Font m_font = { 0 };
	Sound m_click = { 0 };

	std::vector<Button> m_buttons;
public:
	Menu();
	~Menu();

	void awake(GameData& data) override;
	void process(GameData& data) override;
	void render(GameData& data) override;
};

} // namespace sokoboom