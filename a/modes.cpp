#include "modes.h"

int RunEditor()
{
	// WINDOW //
	SetWindowState(FLAG_WINDOW_RESIZABLE);
	InitWindow(800, 600, "tile engine");
	SetExitKey(KEY_INVALID);

	// BUTTONS //
	Button buttons[BUT_NUM];

	// TEXTURES //
	textures.initTextures();
	buttons[0] = LoadTexture("textures/buttons/button_0.png");
	buttons[1] = LoadTexture("textures/buttons/button_1.png");
	//buttons[2] = LoadTexture("textures/buttons/button_2.png");
	//buttons[2] = LoadTexture("buttons/button_2.png");

	const int TILE_NUM = textures.tiles.size();

	// CAM //
	Camera2D cam;
	cam.offset = { 0, 0 };
	cam.rotation = 0;
	cam.target = { 0, 0, };
	cam.zoom = 1;

	// MESH //
	std::vector<Tile> mesh;

	// TILES  //
	std::vector<Tile> tileMap;
	UiTile* UiTiles = new UiTile[TILE_NUM];
	for (int i = 0; i < TILE_NUM; i++)
		UiTiles[i] = i;

	// BUTTONS //
	int xb = 0;
	for (int i = 0; i < BUT_NUM; i++)
	{
		buttons[i].setPos(14 + xb, 6);
		xb += buttons[i].getTexture().width + 1;
	}

	auto lambda1 = [&tileMap]() { Export(tileMap); };
	std::function<void()> func1 = lambda1;
	buttons[0].setFunc(func1);

	auto lambda2 = [&tileMap]() { Import(tileMap); };
	std::function<void()> func2 = lambda2;
	buttons[1].setFunc(func2);

	//auto lambda3 = [&tileMap, &mesh]() { CalculateMesh(tileMap, mesh); };
	//std::function<void()> func3 = lambda3;
	//buttons[2].setFunc(func3);

	// VARIABLES //
	float uiOffset = 0;
	int selected = 0;
	bool uiClick = false;
	bool mouseOver = false;

	// AIR COLOR //
	Image image = LoadImageFromTexture(textures.tiles[0]);
	Color* colors = LoadImageColors(image);
	int index = (4 * image.width) + 4;
	Color pixel = colors[index];

	// GAME LOOP //
	while (!WindowShouldClose())
	{
		// LOGIC //
		Vector2 MousePos = GetScreenToWorld2D(GetMousePosition(), cam);
		float dt = GetFrameTime();

		float xPlace = (float)((int)(MousePos.x / textures.tiles[selected].width) * textures.tiles[selected].width),
			yPlace = (float)((int)(MousePos.y / textures.tiles[selected].height) * textures.tiles[selected].height);

		if (MousePos.x < 0)
			xPlace -= textures.tiles[selected].width;
		if (MousePos.y < 0)
			yPlace -= textures.tiles[selected].height;

		if (IsKeyPressed('M'))
			CalculateMesh(tileMap, mesh);

		// BUTTONS //
		for (int i = 0; i < BUT_NUM; i++)
		{
			buttons[i].Clicked(GetMousePosition());
		}

		// UI //
		for (int i = 0; i < TILE_NUM; i++)
		{
			if (UiTiles[i].Clicked(GetScreenWidth() - 56, i * 64 + 16 + uiOffset, 48)) {
				selected = UiTiles[i].getIndex();
				break;
			}
		}

		if (GetMousePosition().x >= GetScreenWidth() - 64 || GetMousePosition().y < 64)
			uiClick = true;
		else
			uiClick = false;

		// WORLD MOVE //
		if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
			cam.target.x -= GetMouseDelta().x;
			cam.target.y -= GetMouseDelta().y;
		}

		// ZOOM //
		if (GetMouseWheelMove() && !uiClick)
		{
			cam.zoom += GetMouseWheelMove() * dt;
		}
		else if (uiClick)
		{
			uiOffset += GetMouseWheelMove();
		}

		mouseOver = false;

		// ERASE //
		if (selected == 0)
			for (int i = 0; i < tileMap.size(); i++)
			{
				if (tileMap[i].Clicked(textures.tiles[selected].width, textures.tiles[selected].height, MousePos))
					tileMap.erase(tileMap.begin() + i);
			}
		else
			for (int i = 0; i < tileMap.size(); i++)
			{
				if (tileMap[i].Clicked(textures.tiles[selected].width, textures.tiles[selected].height, MousePos, mouseOver))
					tileMap[i].setIndex(selected);
			}

		// PLACE //
		if (!uiClick && selected && !mouseOver) {
			if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
				tileMap.push_back({ selected, Vector2{xPlace, yPlace} });
			}
			//std::cout << tileMap.size() << std::endl;
		}

		// RENDER //
		BeginDrawing();
		// CAM //
		BeginMode2D(cam);
		ClearBackground({ (unsigned char)(pixel.r - 4), (unsigned char)(pixel.g - 4), (unsigned char)(pixel.b - 4), pixel.a });

		for (int i = 0; i < tileMap.size(); i++)
			tileMap[i].DrawTile();

		for (int i = 0; i < mesh.size(); i++)
			DrawRectangleLines(mesh[i].getPos().x, mesh[i].getPos().y, 64, 64, GREEN);

		DrawTexture(textures.tiles[selected], xPlace, yPlace, { 245, 245, 245, 120 });

		EndMode2D();

		// UI //
		DrawRectangle(GetScreenWidth() - 64, 0, 64, GetScreenHeight(), DARKGRAY);
		DrawRectangle(0, 0, GetScreenWidth(), 48, DARKGRAY);

		for (int i = 0; i < TILE_NUM; i++)
		{
			UiTiles[i].DrawTile(GetScreenWidth() - 56, i * 64 + 16 + uiOffset, 0.75f);

			if (UiTiles[i].MouseOver(GetScreenWidth() - 56, i * 64 + 16 + uiOffset, 48))
				DrawRectangleLines(GetScreenWidth() - 56, i * 64 + 16 + uiOffset, textures.tiles[i].width * 0.75f, textures.tiles[i].height * 0.75f, WHITE);
			if (UiTiles[i].getIndex() == selected)
				DrawRectangleLines(GetScreenWidth() - 56, i * 64 + 16 + uiOffset, textures.tiles[i].width * 0.75f, textures.tiles[i].height * 0.75f, GREEN);
		}

		for (int i = 0; i < BUT_NUM; i++)
		{
			buttons[i].DrawButton();
		}

		EndDrawing();
	}

	delete[] UiTiles;

	return 0;
}

int RunGame()
{
	// Init Window //
	SetWindowState(FLAG_WINDOW_RESIZABLE);
	InitWindow(600, 600, "gam");
	SetExitKey(KEY_INVALID);

	// Screen Bounds //
	int screenW = GetScreenWidth();
	int screenH = GetScreenHeight();

	// Variables //
	bool pause = true;

	// Initiate Player //
	Player player;
	player.setCamOffset({ (float)screenW / 2, (float)screenH / 2 });

	// Load Textures //
	player.initTexture();
	textures.initTextures();

	// World and Mesh //
	std::vector<Tile> tileMap;
	std::vector<Tile> mesh;

	// Load World //
	Import(tileMap);

	// Spawn //
	Vector2 spawn = { 0, 0 };
	for (auto tile : tileMap)
		if (tile.getIndex() == 10)
			spawn = { tile.getPos().x, tile.getPos().y - player.getTexture().height };
	player.setPos(spawn);

	// Buttons //
	Button buttons[1];
	buttons[0].setPos(10, 10);
	buttons[0] = LoadTexture("./textures/buttons/respawn_button.png");

	auto lambda1 = [&spawn, &player]() { player.setPos(spawn); };
	std::function<void()> func1 = lambda1;
	buttons[0].setFunc(func1);

	// Calculate Mesh //
	CalculateMesh(tileMap, mesh);

	// Game Loop //
	while (!WindowShouldClose())
	{
		// Delta Time //
		float dt = GetFrameTime();

		// Window Resized //
		if (IsWindowResized())
		{
			pause = true;
			screenW = GetScreenWidth();
			screenH = GetScreenHeight();
			player.setCamOffset({ (float)screenW / 2, (float)screenH / 2 });
		}

		// Pause //
		if (IsKeyPressed(KEY_ESCAPE))
			pause = !pause;

		// Buttons //
		for (int i = 0; i < 1; i++)
			buttons[i].Clicked(GetMousePosition());

		if (!pause)
		{
			float beneathPlayer = 704;

			// Solve Collisions //
			for (int i = 0; i < mesh.size(); i++)
				solveCollision(player, mesh[i], beneathPlayer);

			// Movement //
			if (abs(player.getVel().x) < 2500)
			{
				if (IsKeyDown('A'))
				{
					player.ApplyForce({ -500, 0 }, dt);
				}
				if (IsKeyDown('D'))
				{
					player.ApplyForce({ 500, 0 }, dt);
				}
			}

			// Gravity //
			if (player.getPos().y < beneathPlayer)
				player.ApplyForce({ 0, 980 }, dt);

			// Ground //
			else
			{
				player.setPos({ player.getPos().x, beneathPlayer });
				if (player.getVel().y > 0)
					player.setVel({ player.getVel().x, 0 });

				// Jump //
				if (IsKeyDown(' '))
					player.ApplyForce({ 0, -502 }, 1);
			}

			// Friction //
			player.ApplyForce(mult(player.getVel(), Vector2{ -0.75f, 0 }), dt);

			// Update //
			player.Update(dt);
		}

		if (IsKeyPressed('F'))
			std::cout << GetFPS() << std::endl;

		// Render //
		BeginDrawing();
		ClearBackground(BLACK);
		BeginMode2D(player.getCam());
		// Draw Tiles //
		for (int i = 0; i < tileMap.size(); i++)
			tileMap[i].DrawTile();

		// Draw Player //
		player.Draw(player.getPos().x / 32);

		EndMode2D();

		buttons[0].DrawButton(0.5f);

		if (pause)
			DrawText("Game Paused \nPress ESC to Unpause", screenW / 2, 50, 20, WHITE);

		EndDrawing();
	}
	return 0;
}