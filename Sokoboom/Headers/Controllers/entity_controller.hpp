#pragma once

#include "../Entities/entity.hpp"

#include <memory>
#include <vector>

namespace sokoboom {

class EntityController
{
private:
	std::vector<std::unique_ptr<Entity>> m_entities;
	std::vector<std::size_t> m_free;

public:
	template <typename T>
	struct Handle
	{
		friend EntityController;

		using type = std::size_t;
		type value;

		static constexpr type INVALID = std::numeric_limits<type>::max();

		constexpr Handle() noexcept : value(INVALID) {}

		constexpr explicit operator bool() const noexcept { return value != INVALID; }

	protected:
		explicit Handle(type value) : value(value) {}
	};

	template <typename Ent>
	Handle<Ent> add(Vector2 position = Vector2(0, 0))
	{
		if (!this->m_free.empty()) {
			Handle<Ent> ret(this->m_free.back());
			this->m_free.pop_back();
			this->m_entities[ret.value] = std::make_unique<Ent>(position);
			return ret;
		}

		Handle<Ent> ret(m_entities.size());
		m_entities.emplace_back(std::make_unique<Ent>(position));
		return ret;
	}

	template<typename T>
	void remove()
	{
		/* Remove/free is where all the memory safety issues begin. The index into m_entities serves
		   as the identity represented as Handle<T>, which means they cannot be changed since there 
		   is currently no way to update all outstanding references (Handles).
		   So list processing will now pay a nullptr check because m_entity slots can be empty. This
		   will have to be mitigated with larger refactors to come later.
		*/
		for (std::size_t i = 0, size = this->m_entities.size(); i != size; ++i)
		{
			if (T* out = dynamic_cast<T*>(this->m_entities[i].get()))
			{
				this->m_entities[i].reset();
				this->m_free.emplace_back(i);

				return;
			}
		}
	}

	template <typename Ent>
	Ent& operator[](Handle<Ent> handle)
	{
		return static_cast<Ent&>(*m_entities[handle.value]);
	}

	void process();
	void render();
};

} // namespace sokoboom