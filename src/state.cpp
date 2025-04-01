#include "state.hpp"

#include "data.hpp"
#include "states/end.hpp"
#include "states/game.hpp"
#include "states/menu.hpp"
#include "states/settings.hpp"

namespace sokoboom {

Fade::Fade(State& prev, State& next, Color color, float alpha, float speed)
	: m_state({&prev, &next})
	, m_color(color)
	, m_alpha(alpha)
	, m_speed(speed)
{
}

void Fade::process(GameData& data)
{
	this->m_alpha += this->m_speed * GetFrameTime();
	if (this->m_alpha >= 1)
	{
		this->m_alpha = 1;
		this->m_speed = -this->m_speed;

		m_state[1]->awake(data);
	}
	else if (this->m_alpha <= 0)
	{
		data.state_handler.replace(*m_state[1]);
	}
}

void Fade::render(GameData& data)
{
	this->m_state[this->m_speed < 0]->render(data);
	DrawRectangleV({}, GameData::GAME_SIZE, ::Fade(this->m_color, this->m_alpha));
}

static struct GameStates
{
	Uninit<Menu    > menu    ;
	Uninit<Settings> settings;
	Uninit<Game    > game    ;
	Uninit<End     > end     ;

	void init(GameData& data)
	{
		this->menu    .ctor(    );
		this->settings.ctor(data);
		this->game    .ctor(    );
		this->end     .ctor(data);
	}

	void deinit()
	{
		this->menu    .dtor();
		this->settings.dtor();
		this->game    .dtor();
		this->end     .dtor();
	}

	constexpr State& get(GameState state) noexcept
	{
		switch (state) {
		case GameState::menu    : return this->menu    ;
		case GameState::settings: return this->settings;
		case GameState::game    : return this->game    ;
		case GameState::end     : return this->end     ;
		}

		unreachable();
	}
} g_states;

/*
These kinds of initializations can be problematic because they are not using ctors. This is by design
because it's using static storage that must be initialized at some time during main(). Using ctors 
would invoke ctors prior to main(), which is not a good time.

Another way to prevent explicit init()/deinit() would be to use a bool to track whether initialization
has occurred and always attempt to initialize inside the ctor.
*/
void StateController::init(GameData& data)
{
	g_states.init(data);
}

void StateController::deinit()
{
	g_states.deinit();
}

StateController::StateController()
	: m_state(&g_states.get(GameState{}))
	, m_fade(*m_state, *m_state)
{
	assert(m_state);
}

void StateController::replace(State& state)
{
	this->m_state = &state;
}

void StateController::transition(GameState state)
{
	this->transition(g_states.get(state));
}

void StateController::transition(State& state)
{
	reset(this->m_fade, *this->m_state, state);
	this->replace(this->m_fade);
}

void StateController::process(GameData& data)
{
	this->m_state->process(data);
}

void StateController::render(GameData& data)
{
	this->m_state->render(data);
}

} // namespace sokoboom