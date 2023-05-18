#include "level.h"

Level::Level()
{
	Window();

	InitVariables();

	InitButtons();

	// Import(tileMap); //

	InitPlayer();

	InitTextures();
}

void Level::InitVariables()
{
	chunk = 0;
	screenW = GetScreenWidth();
	screenH = GetScreenHeight();
	pause = true;
}

void Level::InitPlayer()
{
	LoadChunk(world, 0);

	player.setCamOffset({ (float)screenW / 2, (float)screenH / 2 });

	spawn = { 0, generateNoise(0, 0) - 128 };
	//for (const auto& tile : tileMap)
	//	if (tile.getIndex() == 10) {
	//		spawn = { tile.getPos().x, tile.getPos().y - 128 };
	//		break;
	//		//std::cout << tile.getPos().x << std::endl;
	//	}
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
	int farthestChunkPos = 0;
	int farthestChunkNeg = 0;

	Vector2 mousePos = GetScreenToWorld2D(GetMousePosition(), player.getCam());

	bool loaded = false;
	int previousChunk = -1;

	while (!WindowShouldClose())
	{
		// Delta Time //
		dt = GetFrameTime();
		mousePos = GetScreenToWorld2D(GetMousePosition(), player.getCam());

		WindowUpdate();

		KeysPressed();

		for (int i = 0; i < 1; i++)
			buttons[i].Clicked(GetMousePosition());

		if (player.getPos().x >= 0)
			chunk = (int)player.getPos().x / 1024;
		else
			chunk = (int)player.getPos().x / 1024 - 1;

		if (chunk != previousChunk)
		{
			for (int i = chunk - 1; i <= chunk + 1; i++)
				LoadChunk(world, i);

			CalculateMesh(world, mesh, chunk);

			previousChunk = chunk;

			if (chunk > farthestChunkPos)
				farthestChunkPos = chunk;
			else if (chunk < farthestChunkNeg)
				farthestChunkNeg = chunk;
		}

		if (!pause)
		{
			ground = 2048;

			for (int i = 0; i < mesh.size(); i++)
			{
				if (mesh[i].tile().Clicked(mousePos))
				{
					world[mesh[i].chunk()].erase(world[mesh[i].chunk()].begin() + mesh[i].chunkIndex());
					CalculateMesh(world, mesh, chunk);
				}
			}

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
		solveCollision(player, mesh[i].tile(), ground);
}

void Level::Gravity()
{
	// Gravity //
	if (player.getPos().y < ground)
		player.ApplyForce({ 0, 980 }, dt);

	// Ground //
	else
	{
		player.setPos({ player.getPos().x, ground });
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
	for(int c = chunk - 1; c <= chunk + 1; c++)
		for (int i = 0; i < world[c].size(); i++)
			world[c].getChunk()[i].DrawTile();

	for (int i = chunk - 1; i <= chunk + 1; i++)
		DrawLine(i * 1024, -2024, i * 1024, 2048, RED);

	for (int i = 0; i < mesh.size(); i++)
		DrawRectangleLines(mesh[i].tile().getPos().x, mesh[i].tile().getPos().y, 64, 64, GREEN);

	// Draw Player //
	player.Draw(player.getPos().x / 32);

	EndMode2D();

	buttons[0].DrawButton(0.5f);

	if (pause)
		DrawText("Game Paused \nPress ESC to Unpause", screenW / 2, 50, 20, WHITE);

	EndDrawing();
}