#pragma once

#include "../button.hpp"
#include "../data.hpp"
#include "../resource.hpp"
#include "state.hpp"

#include <json.hpp>

#include <raylib.h>
#include <raymath.h>

#include <filesystem>
#include <memory>
#include <vector>

namespace sokoboom {

class Settings : public State
{
private:
	std::vector<Button> m_buttons;

	Sound m_click;
	Font m_font;

	bool m_mute_move = false;
	bool m_mute_sfx = false;
public:
	Settings();

	void awake(GameData& data) override;
	void process(GameData& data) override;
	void render(GameData& data) override;
};

} // namespace sokoboom