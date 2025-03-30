#pragma once
#include <raylib.h>

#include <cassert>
#include <string>
#include <unordered_map>
#include <vector>

namespace sokoboom {

#define SOKOBOOM_X_RESOURCE_TEXTURES(X)\
	X(wall  , "Content/Props/wall.png"     )\
	X(goal  , "Content/Entities/goal.png"  )\
	X(box   , "Content/Entities/box.png"   )\
	X(player, "Content/Entities/player.png")

#define SOKOBOOM_X_RESOURCE_FONTS(X)\
	X(pico8, "Content/pico-8.ttf")

#define SOKOBOOM_X_RESOURCE_SOUNDS(X)\
	X(move   , "Content/Audio/move.wav"     )\
	X(next   , "Content/Audio/next.wav"     )\
	X(explode, "Content/Audio/explosion.wav")\
	X(click  , "Content/Audio/click.wav"    )

template <typename T, typename Deleter>
class Owner : private Deleter
{
private:
	T m_data;

public:
	Owner(T data) : m_data(data) {}

	Owner(const Owner&) = delete;
	void operator=(const Owner&) = delete;

	Owner(Owner&& rhs) noexcept
		: m_data(rhs.m_data)
	{
		rhs.release();
	}
	Owner& operator=(Owner&& rhs) noexcept
	{
		m_data = rhs.m_data;
		rhs.release();
		return *this;
	}

	~Owner()
	{
		(*this)(m_data);
	}

	T& get() { return m_data; }

	T release()
	{
		auto ret = std::move(m_data);
		m_data = T{}; // todo: sketchy
		return ret;
	}
};

class Resource
{
public:
	template <typename T>
	struct Handle
	{
		friend Resource;

		using type = std::size_t;
		type value;

	protected:
		/* Note: this is protected to ensure only Resource constructs these handles as valid. It is not default
			constructible to ensure all clients remember to initialize their resources.
		  
			Lacking default construction can be annoying, so maybe relax this in the future. To do that would require
			making an invalid Handle state*/
		explicit constexpr Handle(type value) noexcept : value(value) {}
	};

private:
	struct Texture2DDeleter
	{
		void operator()(::Texture2D& a) {
			UnloadTexture(a);
		}
	};
	using TextureOwner = Owner<::Texture2D, Texture2DDeleter>;

	struct FontDeleter
	{
		void operator()(Font& a) {
			UnloadFont(a);
		}
	};
	using FontOwner = Owner<::Font, FontDeleter>;

	struct SoundDeleter
	{
		void operator()(Sound& a) {
			UnloadSound(a);
		}
	};
	using SoundOwner = Owner<::Sound, SoundDeleter>;

	std::vector<TextureOwner> m_textures;
	std::vector<FontOwner   > m_fonts   ;
	std::vector<SoundOwner  > m_sounds  ;

	std::unordered_map<std::string, Handle<::Texture2D>> m_index_textures;
	std::unordered_map<std::string, Handle<::Font     >> m_index_fonts   ;
	std::unordered_map<std::string, Handle<::Sound    >> m_index_sounds  ;

public:
	Handle<::Texture2D> texture2d(char const* path);
	Handle<::Font     > font     (char const* path);
	Handle<::Sound    > sound    (char const* path);

	::Texture2D& operator[](Handle<::Texture2D> handle) { return m_textures[handle.value].get(); }
	::Font     & operator[](Handle<::Font     > handle) { return m_fonts   [handle.value].get(); }
	::Sound    & operator[](Handle<::Sound    > handle) { return m_sounds  [handle.value].get(); }

	void release(Handle<::Texture2D>) { /*nop*/ }
	void release(Handle<::Font     >) { /*nop*/ }
	void release(Handle<::Sound    >) { /*nop*/ }

public:
	enum class Texture
	{
#define X(name, path) name,
		SOKOBOOM_X_RESOURCE_TEXTURES(X)
#undef X
	};

	enum class Font
	{
#define X(name, path) name,
		SOKOBOOM_X_RESOURCE_FONTS(X)
#undef X
	};

	enum class Sound
	{
#define X(name, path) name,
		SOKOBOOM_X_RESOURCE_SOUNDS(X)
#undef X
	};

	void load_static_resources()
	{
		assert(m_textures.empty());
		assert(m_fonts   .empty());
		assert(m_sounds  .empty());

#define X(name, path) this->texture2d(path);
		SOKOBOOM_X_RESOURCE_TEXTURES(X)
#undef X

#define X(name, path) this->font(path);
		SOKOBOOM_X_RESOURCE_FONTS(X)
#undef X

#define X(name, path) this->sound(path);
		SOKOBOOM_X_RESOURCE_SOUNDS(X)
#undef X
	}

	::Texture2D& operator[](Texture x) { return m_textures[std::size_t(x)].get(); }
	::Font     & operator[](Font    x) { return m_fonts   [std::size_t(x)].get(); }
	::Sound    & operator[](Sound   x) { return m_sounds  [std::size_t(x)].get(); }

	constexpr Handle<::Texture2D> getHandle(Texture x) const noexcept { return Handle<::Texture2D>(std::size_t(x)); }
	constexpr Handle<::Font     > getHandle(Font    x) const noexcept { return Handle<::Font     >(std::size_t(x)); }
	constexpr Handle<::Sound    > getHandle(Sound   x) const noexcept { return Handle<::Sound    >(std::size_t(x)); }
};

extern Resource resource;

template <typename Handle>
struct HandleReleaser
{
	void operator()(Handle t)
	{
		resource.release(t);
	}
};

template <typename T>
using HandleOwner = Owner<Resource::Handle<T>, HandleReleaser<Resource::Handle<T>>>;

struct Texture2D : HandleOwner<::Texture2D>
{
	using HandleOwner<::Texture2D>::HandleOwner;

	void draw(int posX, int posY, Color tint)
	{
		::DrawTexture(resource[this->get()], posX, posY, tint);
	}

	void draw(Vector2 position, Color tint)
	{
		::DrawTextureV(resource[this->get()], position, tint);
	}
};

struct Font : HandleOwner<::Font>
{
	using HandleOwner<::Font>::HandleOwner;

	Vector2 measure_text_ex(const char *text, float fontSize, float spacing)
	{
		return ::MeasureTextEx(resource[this->get()], text, fontSize, spacing);
	}

	void draw_text_pro(const char *text, Vector2 position, Vector2 origin, float rotation, float fontSize, float spacing, Color tint)
	{
		return ::DrawTextPro(resource[this->get()], text, position, origin, rotation, fontSize, spacing, tint);
	}
};

struct Sound : HandleOwner<::Sound>
{
	using HandleOwner<::Sound>::HandleOwner;

	void operator()()
	{
		::PlaySound(resource[this->get()]);
	}
};

} // namespace sokoboom