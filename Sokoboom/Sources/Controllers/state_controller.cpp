#include "../../Headers/Controllers/state_controller.hpp"

#include "../../Headers/States/end.hpp"
#include "../../Headers/States/game.hpp"
#include "../../Headers/States/menu.hpp"
#include "../../Headers/States/settings.hpp"

#include <optional>

namespace sokoboom {

template <typename T>
union Uninit
{
	char _;
	T data;

	Uninit() {}
	~Uninit() {}

	void ctor() { new (this) T(); }
	void dtor() { this->data.~T(); }

	constexpr T* addr() noexcept { return &this->data; }
	constexpr operator T&() noexcept { return this->data; }
};

struct GameStates
{
	Uninit<Menu    > menu    ;
	Uninit<Settings> settings;
	Uninit<Game    > game1   ;
	Uninit<Game    > game2   ;
	Uninit<End     > end     ;
} g_gameStates;

static constexpr State& allocState(State* current, GameState state)
{
	switch (state) {
	case GameState::menu    : g_gameStates.menu    .ctor(); return g_gameStates.menu    ;
	case GameState::settings: g_gameStates.settings.ctor(); return g_gameStates.settings;
	case GameState::end     : g_gameStates.end     .ctor(); return g_gameStates.end     ;
	case GameState::game    : {
		auto& ret = current != g_gameStates.game1.addr()
			? g_gameStates.game1
			: g_gameStates.game2;
		ret.ctor();
		return ret;
	}
	}

	utilities::unreachable();
}

Color StateController::colour() const
{
	return Fade(BLACK, this->m_opactity);
}

void StateController::set(GameState state)
{
	this->switching = true;
	this->m_temporary.reset(&allocState(this->m_state.get(), state));
	assert(this->m_temporary != this->m_state);
}

void StateController::process(GameData& data)
{
	if (this->switching)
	{
		if (!this->m_reverse)
		{
			this->m_opactity += this->m_speed * GetFrameTime();
			if (this->m_opactity >= 1)
			{
				this->m_reverse = true;
				this->m_opactity = 1;

				this->m_state = std::move(this->m_temporary);
				this->m_state->awake(data);
			}
		}
		else
		{
			this->m_opactity -= this->m_speed * GetFrameTime();
			if (this->m_opactity <= 0)
			{
				this->m_reverse = false;
				this->m_opactity = 0;

				this->switching = false;
			}
		}
	}

	if (this->m_state) this->m_state->process(data);
}

void StateController::render(GameData& data)
{
	if (this->m_state == nullptr) return;
	this->m_state->render(data);
}

} // namespace sokoboom