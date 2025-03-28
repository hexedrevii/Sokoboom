#pragma once

#include "../Controllers/entity_controller.hpp"

namespace sokoboom {

class State
{
public:
	virtual void awake() {};
	virtual void process() {};
	virtual void render() {};
	virtual void leave() {};
};

} // namespace sokoboom