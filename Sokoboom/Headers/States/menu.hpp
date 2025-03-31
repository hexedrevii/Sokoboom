#pragma once

#include "../button.hpp"
#include "../resource.hpp"
#include "state.hpp"

#include <vector>

namespace sokoboom {

struct GameData;

class Menu : public State
{
private:
	static constexpr Resource::Font font = Resource::Font::pico8;

	std::vector<Button> m_buttons;

public:
	explicit Menu(GameData& data);

	void process(GameData& data) override;
	void render(GameData& data) override;
};

} // namespace sokoboom