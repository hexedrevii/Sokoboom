#pragma once

#include "../data.hpp"
#include "../States/state.hpp"
#include "entity_controller.hpp"

#include <memory>

namespace sokoboom {

class StateController
{
private:
	std::unique_ptr<State> m_state = nullptr;
	std::unique_ptr<State> m_temporary = nullptr;

	bool m_reverse = false;

	float m_speed = 5;
	float m_opactity = 0;
public:
	bool switching = false;

	Color colour() const;
	void set(std::unique_ptr<State> state);

	void process(GameData& data);
	void render(GameData& data);
	void leave();
};

inline Color StateController::colour() const
{
	return Fade(BLACK, this->m_opactity);
}

inline void StateController::set(std::unique_ptr<State> state)
{
	this->switching = true;
	this->m_temporary = std::move(state);
}

inline void StateController::process(GameData& data)
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

				if (this->m_state != nullptr)
				{
					this->m_state->leave();
				}

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

	if (this->m_state == nullptr) return;
	this->m_state->process(data);
}

inline void StateController::render(GameData& data)
{
	if (this->m_state == nullptr) return;
	this->m_state->render(data);
}

inline void StateController::leave()
{
	if (this->m_state == nullptr) return;
	this->m_state->leave();

	this->m_state.reset();
}

} // namespace sokoboom