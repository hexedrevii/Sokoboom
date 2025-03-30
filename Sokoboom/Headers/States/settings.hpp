#pragma once

#include "../button.hpp"
#include "../resource.hpp"
#include "state.hpp"

#include <vector>

namespace sokoboom {

struct GameData;

class Settings : public State
{
private:
	static constexpr Resource::Font font = Resource::Font::pico8;
	
	std::vector<Button> m_buttons;

	// todo: pair with GameData. There's no "commit", so these seem redundant.
	bool m_mute_move = false;
	bool m_mute_sfx = false;

public:
	void awake(GameData& data) override;
	void process(GameData& data) override;
	void render(GameData& data) override;
};

} // namespace sokoboom