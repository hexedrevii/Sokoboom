#pragma once

#include "../Controllers/entity_controller.hpp"

namespace sokoboom {

struct GameData;

class State
{
public:
	virtual void awake(GameData& /*data*/) {}
	virtual void process(GameData& /*data*/) {}
	virtual void render(GameData& /*data*/) {}
};

} // namespace sokoboom