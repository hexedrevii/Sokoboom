#include "../../Headers/Controllers/entity_controller.hpp"

namespace sokoboom {

void EntityController::process()
{
	for (const std::unique_ptr<Entity>& entity : this->m_entities)
	{
		if (entity) entity->process();
	}
}

void EntityController::render()
{
	for (const std::unique_ptr<Entity>& entity : this->m_entities)
	{
		if (entity) entity->render();
	}
}

} // namespace sokoboom