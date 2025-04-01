#pragma once
#include <raylib.h>

#include <cassert>
#include <string>
#include <string_view>
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
	/*implicit*/ Owner(T data) : m_data(data) {}

	Owner(const Owner&) = delete;
	void operator=(const Owner&) = delete;

	Owner(Owner&& rhs) noexcept
		: m_data(rhs.m_data)
	{
		rhs.release();
	}
	Owner& operator=(Owner&& rhs) noexcept
	{
		this->m_data = rhs.m_data;
		rhs.release();
		return *this;
	}

	~Owner()
	{
		this->destroy();
	}

	T& get() { return this->m_data; }

	T release()
	{
		auto ret = std::move(m_data);
		this->m_data = T{};
		return ret;
	}

	void destroy() noexcept
	{
		(*this)(this->m_data);
	}
};

struct Texture2D;
struct Font;
struct Sound;

class Resource
{
public:
	template <typename T>
	struct Handle
	{
		friend class Resource;

		using Type = std::size_t;
		static constexpr Type invalid = std::numeric_limits<Type>::max();
		Type value = invalid;

		constexpr Handle() noexcept = default;

		explicit constexpr operator bool() const noexcept { return this->value != invalid; }

	protected:
		explicit constexpr Handle(Type value) noexcept : value(value) {}
	};

	struct fixed {
		enum class Texture : Handle<::Texture2D>::Type
		{
		#define X(name, path) name,
			SOKOBOOM_X_RESOURCE_TEXTURES(X)
		#undef X
		};

		enum class Font : Handle<::Font>::Type
		{
		#define X(name, path) name,
			SOKOBOOM_X_RESOURCE_FONTS(X)
		#undef X
		};

		enum class Sound : Handle<::Sound>::Type
		{
		#define X(name, path) name,
			SOKOBOOM_X_RESOURCE_SOUNDS(X)
		#undef X
		};
	};

private:
	struct Texture2DDeleter
	{
		void operator()(::Texture2D& a) noexcept
		{
			::UnloadTexture(a);
		}
	};
	using TextureOwner = Owner<::Texture2D, Texture2DDeleter>;

	struct FontDeleter
	{
		void operator()(::Font& a) noexcept
		{
			::UnloadFont(a);
		}
	};
	using FontOwner = Owner<::Font, FontDeleter>;

	struct SoundDeleter
	{
		void operator()(::Sound& a) noexcept
		{
			::UnloadSound(a);
		}
	};
	using SoundOwner = Owner<::Sound, SoundDeleter>;

	std::vector<TextureOwner> m_textures;
	std::vector<FontOwner   > m_fonts   ;
	std::vector<SoundOwner  > m_sounds  ;

	std::vector<std::size_t> m_rc_textures;
	std::vector<std::size_t> m_rc_fonts   ;
	std::vector<std::size_t> m_rc_sounds  ;

	struct StringHash
	{
		using is_transparent = void;
		std::size_t operator()(const char*        s) const { return std::hash<std::string_view>{}(s); }
		std::size_t operator()(std::string_view   s) const { return std::hash<std::string_view>{}(s); }
		std::size_t operator()(const std::string& s) const { return std::hash<std::string     >{}(s); }
	};

	std::unordered_map<std::string, Handle<::Texture2D>, StringHash, std::equal_to<>> m_index_textures;
	std::unordered_map<std::string, Handle<::Font     >, StringHash, std::equal_to<>> m_index_fonts   ;
	std::unordered_map<std::string, Handle<::Sound    >, StringHash, std::equal_to<>> m_index_sounds  ;

public:
#ifndef NDEBUG
	~Resource();
#endif

	void load();
	void unload();

	Texture2D texture2d(std::string_view path);
	Font      font     (std::string_view path);
	Sound     sound    (std::string_view path);

	::Texture2D& operator[](Handle<::Texture2D> handle) { return m_textures[handle.value].get(); }
	::Font     & operator[](Handle<::Font     > handle) { return m_fonts   [handle.value].get(); }
	::Sound    & operator[](Handle<::Sound    > handle) { return m_sounds  [handle.value].get(); }

	void release(Handle<::Texture2D> handle);
	void release(Handle<::Font     > handle);
	void release(Handle<::Sound    > handle);

	::Texture2D& operator[](fixed::Texture x) { return m_textures[Handle<::Texture>::Type(x)].get(); }
	::Font     & operator[](fixed::Font    x) { return m_fonts   [Handle<::Font   >::Type(x)].get(); }
	::Sound    & operator[](fixed::Sound   x) { return m_sounds  [Handle<::Sound  >::Type(x)].get(); }

	constexpr Handle<::Texture2D> handle(fixed::Texture x) noexcept { Handle<::Texture2D> ret{Handle<::Texture>::Type(x)}; m_rc_textures[ret.value] += 1; return ret; }
	constexpr Handle<::Font     > handle(fixed::Font    x) noexcept { Handle<::Font     > ret{Handle<::Font   >::Type(x)}; m_rc_fonts   [ret.value] += 1; return ret; }
	constexpr Handle<::Sound    > handle(fixed::Sound   x) noexcept { Handle<::Sound    > ret{Handle<::Sound  >::Type(x)}; m_rc_sounds  [ret.value] += 1; return ret; }
};

extern Resource resource;

template <typename Handle>
struct HandleReleaser
{
	void operator()(Handle& t) noexcept
	{
		if (t)
		{
			resource.release(t);
			t = Handle{};
		}
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