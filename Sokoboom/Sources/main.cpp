#include "../Headers/data.hpp"
#include "../Headers/map.hpp"
#include "../Headers/resource.hpp"
#include "../Headers/utilities.hpp"

#include <raylib.h>
#include <raymath.h>

#include <json.hpp>

#include <cmath>
#include <fstream>
#include <iostream>

namespace sokoboom {

// todo: global for now. Can always inject Resource& into every State that requires resources
Resource resource;

void run(GameData& data, RenderTexture2D& renderer)
{
	while (!data.exit)
	{
		if (WindowShouldClose())
		{
			data.exit = true;
			break;
		}

		// Screen scale
		const float scale = fmin(
			GetScreenWidth() / GameData::GAME_SIZE.x,
			GetScreenHeight() / GameData::GAME_SIZE.y
		);

		const Vector2 mouse = GetMousePosition();
		const Vector2 unclamped = Vector2(
			(mouse.x - (GetScreenWidth() - (GameData::GAME_SIZE.x * scale)) * 0.5f) / scale,
			(mouse.y - (GetScreenHeight() - (GameData::GAME_SIZE.y * scale)) * 0.5f) / scale
		);

		data.virtual_mouse = Vector2Clamp(unclamped, Vector2Zero(), GameData::GAME_SIZE);

		data.state_handler.process(data);

		BeginTextureMode(renderer);
		{
			data.state_handler.render(data);
		}
		EndTextureMode();

		BeginDrawing();
		{
			ClearBackground(BLACK);

			// Fucked up screen scaling calculations
			DrawTexturePro(
				renderer.texture,
				Rectangle(0, 0, float(renderer.texture.width), float(-renderer.texture.height)),
				Rectangle(
					(GetScreenWidth() - (GameData::GAME_SIZE.x * scale)) * 0.5f,
					(GetScreenHeight() - (GameData::GAME_SIZE.y * scale)) * 0.5f,
					GameData::GAME_SIZE.x * scale, GameData::GAME_SIZE.y * scale
				),
				Vector2Zero(), 0, WHITE
			);
		}
		EndDrawing();
	}
}

void main()
{
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(800, 600, "Sokoboom");
	SetExitKey(KEY_NULL);

	InitAudioDevice();

	RenderTexture2D renderer = LoadRenderTexture(trunc(GameData::GAME_SIZE.x), trunc(GameData::GAME_SIZE.y));
	SetTextureFilter(renderer.texture, TEXTURE_FILTER_POINT);

	std::cout << "INFO: Tile size: " << GameData::TILE_SIZE << "\n";
	std::cout << "INFO: Game resolution: " << GameData::GAME_SIZE.x << ", " << GameData::GAME_SIZE.y << "\n";

	resource.load();
	{
		GameData data;
		StateController::init(data);
		std::cout << "INFO: Muted SFX: " << data.mute_sfx << "\n";
		std::cout << "INFO: Muted Move: " << data.mute_move << "\n";

		data.load_settings();
		data.load_maps(); // todo: generalized map loading

		run(data, renderer);
		StateController::deinit();
	}
	resource.unload();

	CloseAudioDevice();
	CloseWindow();
}

} // namespace sokoboom

// Disable terminal in release mode only
#ifndef _DEBUG
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif // !_DEBUG

int main()
{
	sokoboom::main();
}