#pragma once

#include "../States/state.hpp"

#include <raylib.h>

#include <memory>

namespace sokoboom {

struct GameData;
class State;

enum class GameState
{
	menu,
	settings,
	game,
	end,
};

class StateController
{
private:
	struct Deleter
	{
		void operator()(State* s) { s->~State(); }
	};
	using StatePtr = std::unique_ptr<State, Deleter>;
	StatePtr m_state = nullptr;
	StatePtr m_temporary = nullptr;

	bool m_reverse = false;

	float m_speed = 5;
	float m_opactity = 0;
	bool m_switching = false;

public:
	void set(GameData& data, GameState state);

	void process(GameData& data);
	void render(GameData& data);
};

} // namespace sokoboom