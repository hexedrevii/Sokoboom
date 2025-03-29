#pragma once

#include "../Entities/entity.hpp"

#include <memory>
#include <vector>

namespace sokoboom {

class EntityController
{
private:
	std::vector<std::unique_ptr<Entity>> m_entities;

public:
	template<typename T>
	void remove()
	{
		for (const std::unique_ptr<Entity>& entity : this->m_entities)
		{
			if (T* out = dynamic_cast<T*>(entity.get()))
			{
				this->m_entities.erase(
					std::find(this->m_entities.begin(), this->m_entities.end(), entity)
				);

				return;
			}
		}
	}

	void add(std::unique_ptr<Entity> e);

	void process();
	void render();
};

} // namespace sokoboom