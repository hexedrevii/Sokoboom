#include "../../Headers/Controllers/entity_controller.hpp"

#include "../../Headers/Entities/entity.hpp"
#include "../../Headers/Entities/player.hpp"

namespace sokoboom {

EntityController::Handle<Player> EntityController::addPlayer(
	std::function<void(Vector2, Direction)> on_player_moved,
	Vector2 position)
{
	return this->create<Player>(std::move(on_player_moved), position);
}

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