#pragma once

#include "../button.hpp"
#include "../data.hpp"
#include "../utilities.hpp"
#include "state.hpp"

#include <raylib.h>
#include <raymath.h>

#include <filesystem>
#include <memory>

namespace sokoboom {

class End : public State
{
private:
	Font m_font = { 0 };
	Sound m_click = { 0 };

	std::vector<Button> m_buttons;
public:
	End();
	~End();

	void awake(GameData& data) override;
	void process(GameData& data) override;
	void render(GameData& data) override;
};

} // namespace sokoboom