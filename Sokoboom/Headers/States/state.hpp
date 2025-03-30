#pragma once

namespace sokoboom {

struct GameData;

class State
{
public:
	virtual ~State() = default;
	virtual void awake(GameData& /*data*/) {}
	virtual void process(GameData& /*data*/) {}
	virtual void render(GameData& /*data*/) {}
};

} // namespace sokoboom