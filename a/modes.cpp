#include "modes.h"

// Editor Class //
Editor::Editor()
{
	Window();

	InitTextures();

	InitUi();

	InitCamera();

	InitButtons();

	CalculateColor();
}

void Editor::InitCamera()
{
	cam.offset = { 0, 0 };
	cam.rotation = 0;
	cam.target = { 0, 0, };
	cam.zoom = 1;
}

void Editor::InitVariables()
{
	uiOffset = 0;
	selected = 0;
	uiClick = false;
	mouseOver = false;
}

void Editor::InitButtons()
{
	auto lambda1 = [&]() { Export(tileMap); };
	buttons[0].setFunc(std::function<void()>(lambda1));

	auto lambda2 = [&]() { Import(tileMap); };
	buttons[1].setFunc(std::function<void()>(lambda2));

	auto lambda3 = [&]() { textures.reload(); };
	buttons[2].setFunc(std::function<void()>(lambda3));

	int xb = 0;
	for (int i = 0; i < BUT_NUM; i++)
	{
		buttons[i].setPos(14 + xb, 6);
		xb += buttons[i].getTexture().width + 1;
	}
}

void Editor::InitTextures()
{
	textures.initTextures();
	buttons[0] = LoadTexture("textures/buttons/button_0.png");
	buttons[1] = LoadTexture("textures/buttons/button_1.png");
	buttons[2] = LoadTexture("textures/buttons/refresh.png");
	TILE_NUM = textures.tiles.size();
}

void Editor::InitUi()
{
	UiTiles = new UiTile[TILE_NUM];
	for (int i = 0; i < TILE_NUM; i++)
		UiTiles[i] = i;
}

void Editor::CalculateColor()
{
	Image image = LoadImageFromTexture(textures.tiles[0].texture);
	Color* colors = LoadImageColors(image);
	int index = (4 * image.width) + 4;
	pixel = colors[index];
	pixel.r -= 4;
	pixel.g -= 4;
	pixel.b -= 4;
	UnloadImage(image);
	delete colors;
}

void Editor::Run()
{
	while (!WindowShouldClose())
	{
		UpdateVars();

		WindowUpdate();

		KeysPressed();

		Buttons();

		CalcUi();

		WorldMove();

		Zoom();

		mouseOver = false;

		Erase();

		Place();

		Render();
	}

	Unload();
}

void Editor::UpdateVars()
{
	MousePos = GetScreenToWorld2D(GetMousePosition(), cam);
	dt = GetFrameTime();
}

void Editor::KeysPressed()
{
	if (IsKeyPressed('M'))
		CalculateMesh(tileMap, mesh);
}

void Editor::Buttons()
{
	for (int i = 0; i < BUT_NUM; i++)
	{
		buttons[i].Clicked(GetMousePosition());
	}
}

void Editor::CalcUi()
{
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
}

void Editor::WorldMove()
{
	if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
		cam.target.x -= GetMouseDelta().x / cam.zoom;
		cam.target.y -= GetMouseDelta().y / cam.zoom;
	}
}

void Editor::Zoom()
{
	if (GetMouseWheelMove() && !uiClick)
	{
		cam.zoom += GetMouseWheelMove() * cam.zoom / 10;
	}
	else if (uiClick)
	{
		uiOffset += GetMouseWheelMove();
	}
}

void Editor::WindowUpdate()
{
	if (IsWindowResized())
	{
		cam.offset.x = GetScreenWidth() * 0.5f;
		cam.offset.y = GetScreenHeight() * 0.5f;
	}
}

void Editor::Erase()
{
	if (selected == 0)
		for (int i = 0; i < tileMap.size(); i++)
		{
			if (tileMap[i].Clicked(Gtexture(selected).width, Gtexture(selected).height, MousePos))
				tileMap.erase(tileMap.begin() + i);
		}
	else
		for (int i = 0; i < tileMap.size(); i++)
		{
			if (tileMap[i].Clicked(Gtexture(selected).width, Gtexture(selected).height, MousePos, mouseOver))
				tileMap[i].setIndex(selected);
		}
}

void Editor::Place()
{
	xPlace = (float)((int)(MousePos.x / Gtexture(selected).width) * Gtexture(selected).width);
	yPlace = (float)((int)(MousePos.y / Gtexture(selected).height) * Gtexture(selected).height);

	if (MousePos.x < 0)
		xPlace -= textures.tiles[selected].texture.width;
	if (MousePos.y < 0)
		yPlace -= textures.tiles[selected].texture.height;

	if (!uiClick && selected && !mouseOver)
	{
		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
		{
			tileMap.push_back({ selected, Vector2{xPlace, yPlace} });
		}
	}
}

void Editor::Render()
{
	BeginDrawing();
	// CAM //
	BeginMode2D(cam);
	ClearBackground(pixel);

	for (int i = 0; i < tileMap.size(); i++)
		tileMap[i].DrawTile();

	for (int i = 0; i < mesh.size(); i++)
		DrawRectangleLines(mesh[i].getPos().x, mesh[i].getPos().y, Gsize(mesh[i].getIndex()).x, Gsize(mesh[i].getIndex()).y, GREEN);

	DrawTexture(Gtexture(selected), xPlace, yPlace, { 245, 245, 245, 120 });

	EndMode2D();

	// UI //
	DrawRectangle(GetScreenWidth() - 64, 0, 64, GetScreenHeight(), DARKGRAY);
	DrawRectangle(0, 0, GetScreenWidth(), 48, DARKGRAY);

	for (int i = 0; i < TILE_NUM; i++)
	{
		UiTiles[i].DrawTile(GetScreenWidth() - 56, i * 64 + 16 + uiOffset, 0.75f);

		if (UiTiles[i].MouseOver(GetScreenWidth() - 56, i * 64 + 16 + uiOffset, Gsize(i) * 0.75f))
			DrawRectangleLines(GetScreenWidth() - 56, i * 64 + 16 + uiOffset, Gtexture(i).width * 0.75f, Gtexture(i).height * 0.75f, WHITE);
		if (UiTiles[i].getIndex() == selected)
			DrawRectangleLines(GetScreenWidth() - 56, i * 64 + 16 + uiOffset, Gtexture(i).width * 0.75f, Gtexture(i).height * 0.75f, GREEN);
	}

	for (int i = 0; i < BUT_NUM; i++)
	{
		buttons[i].DrawButton();
	}

	EndDrawing();
}

void Editor::Unload()
{
	textures.unload();
	delete[] UiTiles;
	for (int i = 0; i < BUT_NUM; i++)
		buttons[i].Unload();
}

// Level Class //
Level::Level()
{
	Window();

	InitVariables();

	InitPlayer();

	InitTextures();

	InitButtons();

	Import(tileMap);

	CalculateMesh(tileMap, mesh);
}

void Level::InitVariables()
{
	screenW = GetScreenWidth();
	screenH = GetScreenHeight();
	pause = true;
}

void Level::InitPlayer()
{
	player.setCamOffset({ (float)screenW / 2, (float)screenH / 2 });

	spawn = { 0, 0 };
	for (const auto& tile : tileMap)
		if (tile.getIndex() == 10)
			spawn = { tile.getPos().x, tile.getPos().y - player.getTexture().height };
	player.setPos(spawn);
}

void Level::InitTextures()
{
	player.initTexture();
	textures.initTextures();
}

void Level::InitButtons()
{
	buttons[0].setPos(10, 10);
	buttons[0] = LoadTexture("./textures/buttons/respawn_button.png");

	auto lambda1 = [&]() { player.setPos(spawn); };
	std::function<void()> func1 = lambda1;
	buttons[0].setFunc(func1);
}

void Level::Run()
{
	while (!WindowShouldClose())
	{
		// Delta Time //
		dt = GetFrameTime();

		WindowUpdate();

		KeysPressed();

		for (int i = 0; i < 1; i++)
			buttons[i].Clicked(GetMousePosition());

		if (!pause)
		{
			beneathPlayer = 704;

			SolveCollisions();

			Gravity();

			// Friction //
			player.ApplyForce(mult(player.getVel(), Vector2{ -0.75f, 0 }), dt);

			player.Update(dt);
		}

		Render();
	}
	textures.unload();
}

void Level::WindowUpdate()
{
	if (IsWindowResized())
	{
		pause = true;
		screenW = GetScreenWidth();
		screenH = GetScreenHeight();
		player.setCamOffset({ (float)screenW / 2, (float)screenH / 2 });
	}
}

void Level::KeysPressed()
{
	// Pause //
	if (IsKeyPressed(KEY_ESCAPE))
		pause = !pause;

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

	// Frames //
	if (IsKeyPressed('F'))
		std::cout << GetFPS() << std::endl;
}

void Level::SolveCollisions()
{
	for (int i = 0; i < mesh.size(); i++)
		solveCollision(player, mesh[i], beneathPlayer);
}

void Level::Gravity()
{
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
}

void Level::Render()
{
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