#include "../../Headers/resource.hpp"

#include "../../Headers/utilities.hpp"

#include <filesystem>

namespace sokoboom {

namespace {
	::Texture2D load_texture2d_relative(const std::filesystem::path& path)
	{
		std::filesystem::path full = GetApplicationDirectory() / path;
		std::string legacy = full.string();

		if (!FileExists(legacy.c_str()))
		{
			return { 0 };
		}

		return LoadTexture(legacy.c_str());
	}

	::Font load_font_relative(const std::filesystem::path& path)
	{
		std::filesystem::path full = GetApplicationDirectory() / path;
		std::string legacy = full.string();

		if (!FileExists(legacy.c_str()))
		{
			return { 0 };
		}

		return LoadFontEx(legacy.c_str(), 100, 0, 0);
	}

	::Sound load_sound_relative(const std::filesystem::path& path)
	{
		std::filesystem::path full = GetApplicationDirectory() / path;
		std::string legacy = full.string();

		if (!FileExists(legacy.c_str()))
		{
			return { 0 };
		}

		return LoadSound(legacy.c_str());
	}
}

Resource::Handle<::Texture2D> Resource::texture2d(char const* path)
{
	auto i = m_index_textures.find(path);
	if (i != m_index_textures.end())
		return i->second;

	Handle<::Texture2D> ret { m_textures.size() };
	m_textures.emplace_back(TextureOwner(load_texture2d_relative(path)));
	m_index_textures.insert({path, ret});
	return ret;
}

Resource::Handle<::Font> Resource::font(char const* path)
{
	auto i = m_index_fonts.find(path);
	if (i != m_index_fonts.end())
		return i->second;

	Handle<::Font> ret { m_fonts.size() };
	m_fonts.emplace_back(FontOwner(load_font_relative(path)));
	m_index_fonts.insert({path, ret});
	return ret;
}

Resource::Handle<::Sound> Resource::sound(char const* path)
{
	auto i = m_index_sounds.find(path);
	if (i != m_index_sounds.end())
		return i->second;

	Handle<::Sound> ret { m_sounds.size() };
	m_sounds.emplace_back(SoundOwner(load_sound_relative(path)));
	m_index_sounds.insert({path, ret});
	return ret;
}

} // namespace sokoboom