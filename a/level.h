#pragma once

#include "headers.h"
#include "textures.h"
#include "player.h"
#include "tile.h"
#include "utils.h"
#include "gui.h"
#include "globals.h"
#include "world.h"
#include "chunk.h"
#include "mesh.h"

class Level
{
public:
	Level();

	void InitVariables();

	void InitPlayer();

	void InitTextures();

	void InitButtons();

	void Run();

	void WindowUpdate();

	void KeysPressed();

	void SolveCollisions();

	void Gravity();

	void Render();

private:
	int screenW;
	int screenH;

	int chunk;

	bool pause;

	float ground;
	float dt;

	Player player;
	Vector2 spawn;

	Button buttons[1];

	std::map<int, Chunk> world;
	std::vector<MeshObj> mesh;
};