#include "resource.hpp"

#include "utilities.hpp"

#include <filesystem>

namespace sokoboom {

#ifndef NDEBUG
Resource::~Resource()
{
	assert(m_textures.empty());
	assert(m_fonts   .empty());
	assert(m_sounds  .empty());
}
#endif

void Resource::load()
{
	assert(m_textures.empty());
	assert(m_fonts   .empty());
	assert(m_sounds  .empty());

#define X(name, path) this->texture2d(path).release();
	SOKOBOOM_X_RESOURCE_TEXTURES(X)
#undef X

#define X(name, path) this->font(path).release();
	SOKOBOOM_X_RESOURCE_FONTS(X)
#undef X

#define X(name, path) this->sound(path).release();
	SOKOBOOM_X_RESOURCE_SOUNDS(X)
#undef X
}

void Resource::unload()
{
#define X(name, path) this->release(Handle<::Texture>{Handle<::Texture>::Type(fixed::Texture::name)});
	SOKOBOOM_X_RESOURCE_TEXTURES(X)
#undef X

#define X(name, path) this->release(Handle<::Font>{Handle<::Texture>::Type(fixed::Font::name)});
	SOKOBOOM_X_RESOURCE_FONTS(X)
#undef X

#define X(name, path) this->release(Handle<::Sound>{Handle<::Texture>::Type(fixed::Sound::name)});
	SOKOBOOM_X_RESOURCE_SOUNDS(X)
#undef X

	for (auto rc : m_rc_textures) assert(rc == 0);
	for (auto rc : m_rc_fonts   ) assert(rc == 0);
	for (auto rc : m_rc_sounds  ) assert(rc == 0);

	m_textures.clear(); m_rc_textures.clear(); m_index_textures.clear();
	m_fonts   .clear(); m_rc_fonts   .clear(); m_index_fonts   .clear();
	m_sounds  .clear(); m_rc_sounds  .clear(); m_index_sounds  .clear();
}

namespace {
	::Texture2D load_texture2d_relative(std::string_view path)
	{
		const std::filesystem::path full = GetApplicationDirectory() / std::filesystem::path(path);
		return ::LoadTexture(full.string().c_str());
	}

	::Font load_font_relative(std::string_view path)
	{
		const std::filesystem::path full = GetApplicationDirectory() / std::filesystem::path(path);
		return ::LoadFontEx(full.string().c_str(), 100, 0, 0);
	}

	::Sound load_sound_relative(std::string_view path)
	{
		const std::filesystem::path full = GetApplicationDirectory() / std::filesystem::path(path);
		return ::LoadSound(full.string().c_str());
	}
}

#define IMPL(NAME, TYPE, OWNER, LOAD)\
	auto i = m_index_##NAME.find(path);\
	if (i != m_index_##NAME.end())\
	{\
		m_rc_##NAME[i->second.value] += 1;\
		return i->second;\
	}\
\
	Handle<TYPE> ret { m_##NAME.size() };\
	m_##NAME.emplace_back(OWNER(LOAD(path)));\
	m_rc_##NAME.emplace_back(1);\
	m_index_##NAME.insert({std::string(path), ret});\
	return ret;

Texture2D Resource::texture2d(std::string_view path) { IMPL(textures, ::Texture2D, TextureOwner, load_texture2d_relative) }
Font      Resource::font     (std::string_view path) { IMPL(fonts   , ::Font     , FontOwner   , load_font_relative     ) }
Sound     Resource::sound    (std::string_view path) { IMPL(sounds  , ::Sound    , SoundOwner  , load_sound_relative    ) }
#undef IMPL

// todo: properly release resource
// It doesn't make much sense to release the resources when there's so few. Just correctly track the reference counts
// and know that rc == 0 means to free the resource.
void Resource::release(Handle<::Texture2D> handle) { auto& rc = m_rc_textures[handle.value]; assert(rc > 0); rc -= 1; /*if (rc == 0) m_textures[handle.value].destroy();*/ }
void Resource::release(Handle<::Font     > handle) { auto& rc = m_rc_fonts   [handle.value]; assert(rc > 0); rc -= 1; /*if (rc == 0) m_fonts   [handle.value].destroy();*/ }
void Resource::release(Handle<::Sound    > handle) { auto& rc = m_rc_sounds  [handle.value]; assert(rc > 0); rc -= 1; /*if (rc == 0) m_sounds  [handle.value].destroy();*/ }

} // namespace sokoboom