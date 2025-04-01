#include "../../Headers/state_controller.hpp"

#include "../../Headers/States/end.hpp"
#include "../../Headers/States/game.hpp"
#include "../../Headers/States/menu.hpp"
#include "../../Headers/States/settings.hpp"

namespace sokoboom {

template <typename T>
union Uninit
{
	char _;
	T data;

	Uninit() : _{} {}
	~Uninit() {}

	template <typename... Args>
	void ctor(Args&&... args) { new (this) T(std::forward<Args>(args)...); }
	void dtor() { this->data.~T(); }

	constexpr T* operator&() noexcept { return &this->data; }
	constexpr operator T&() noexcept { return this->data; }
	constexpr T* operator->() noexcept { return &this->data; }
};

static struct GameStates
{
	Uninit<Menu    > menu    ;
	Uninit<Settings> settings;
	Uninit<Game    > game1   ;
	Uninit<Game    > game2   ; // todo: get rid of this extra game state (change transition logic)
	Uninit<End     > end     ;
} g_states;

static constexpr State& createState(GameData& data, State* current, GameState state)
{
	switch (state) {
	case GameState::menu    : g_states.menu    .ctor(    ); return g_states.menu    ;
	case GameState::settings: g_states.settings.ctor(data); return g_states.settings;
	case GameState::end     : g_states.end     .ctor(data); return g_states.end     ;
	case GameState::game    : {
		auto& ret = current != &g_states.game1
			? g_states.game1
			: g_states.game2;
		ret.ctor(data);
		return ret;
	}
	}

	unreachable();
}

void StateController::set(GameData& data, GameState state)
{
	this->m_switching = true;
	this->m_temporary.reset(&createState(data, this->m_state.get(), state));
	assert(this->m_temporary != this->m_state);
}

void StateController::process(GameData& data)
{
	if (this->m_switching)
	{
		if (!this->m_reverse)
		{
			this->m_opactity += this->m_speed * GetFrameTime();
			if (this->m_opactity >= 1)
			{
				this->m_reverse = true;
				this->m_opactity = 1;

				this->m_state = std::move(this->m_temporary);
			}
		}
		else
		{
			this->m_opactity -= this->m_speed * GetFrameTime();
			if (this->m_opactity <= 0)
			{
				this->m_reverse = false;
				this->m_opactity = 0;

				this->m_switching = false;
			}
		}
	}

	if (this->m_state) this->m_state->process(data);
}

void StateController::render(GameData& data)
{
	if (this->m_state) this->m_state->render(data);

	if ( m_switching )
	{
		DrawRectangleV({}, GameData::GAME_SIZE, Fade(BLACK, this->m_opactity));
	}
}

} // namespace sokoboom