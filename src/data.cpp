#include "data.hpp"

#include <json.hpp>

#include <filesystem>
#include <fstream>

namespace sokoboom {

void GameData::load_settings(std::string_view path)
{
	if (std::ifstream f{GetApplicationDirectory() / std::filesystem::path(path)})
	{
		nlohmann::json json = nlohmann::json::parse(f);

		this->mute_sfx = json["mute_sfx"];
		this->mute_move = json["mute_move"];
	}
}

void GameData::save_settings(std::string_view path)
{
	if (std::ofstream f{GetApplicationDirectory() / std::filesystem::path(path)})
	{
		nlohmann::json json;
		json["mute_sfx"] = this->mute_sfx;
		json["mute_move"] = this->mute_move;

		f << json.dump(4);
	}
}

void GameData::load_maps()
{
	this->maps.emplace_back("INTRO", Map("Content/Maps/intro.p8m"  ));
	this->maps.emplace_back("1"    , Map("Content/Maps/one.p8m"    ));
	this->maps.emplace_back("2"    , Map("Content/Maps/two.p8m"    ));
	this->maps.emplace_back("3"    , Map("Content/Maps/three.p8m"  ));
	this->maps.emplace_back("4"    , Map("Content/Maps/four.p8m"   ));
	this->maps.emplace_back("5"    , Map("Content/Maps/five.p8m"   ));
	this->maps.emplace_back("6"    , Map("Content/Maps/six.p8m"    ));
	this->maps.emplace_back("7"    , Map("Content/Maps/seven.p8m"  ));
	this->maps.emplace_back("8"    , Map("Content/Maps/eight.p8m"  ));
	this->maps.emplace_back("9"    , Map("Content/Maps/nine.p8m"   ));
	this->maps.emplace_back("10"   , Map("Content/Maps/ten.p8m"    ));
	this->maps.emplace_back("END"  , Map("Content/Maps/the_end.p8m"));
}

} // namespace sokoboom