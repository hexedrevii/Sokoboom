#pragma once

#include <raylib.h>

#include <array>

namespace sokoboom {

struct GameData;

enum class GameState
{
	menu,
	settings,
	game,
	end,
};

class State
{
public:
	virtual ~State() = default;
	virtual void awake  (GameData& /*data*/) {}
	virtual void process(GameData& /*data*/) {}
	virtual void render (GameData& /*data*/) {}
};

class Fade : public State
{
private:
	static constexpr auto default_color = BLACK;
	static constexpr auto default_alpha = 0.7f;
	static constexpr auto default_speed = 5.0f;

	Color m_color = default_color;
	float m_alpha = default_alpha;
	float m_speed = default_speed;

	std::array<State*, 2> m_state {};

public:
	explicit Fade(
		State& prev, State& next,
		Color color = default_color,
		float alpha = default_alpha,
		float speed = default_speed);

	void process(GameData& data) override;
	void render(GameData& data) override;
};

class StateController
{
private:
	State* m_state = nullptr; // m_state != nullptr
	Fade m_fade;

public:
	static void init(GameData& data);
	static void deinit();

	StateController();

	void replace(State& state);

	void transition(GameState state);
	void transition(State& state);

	void process(GameData& data);
	void render(GameData& data);
};

} // namespace sokoboom