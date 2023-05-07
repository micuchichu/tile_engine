#pragma once
#include "headers.h"
#include "textures.h"
#include "player.h"
#include "tile.h"
#include "utils.h"
#include "gui.h"
#include "globals.h"
#include "buttons.h"

#define BUT_NUM 3

class Editor
{
public:
	Editor();

	void InitCamera();

	void InitVariables();

	void InitButtons();

	void InitTextures();

	void InitUi();

	void CalculateColor();

	void Run();

	void UpdateVars();

	void KeyPressed();

	void Buttons();

	void CalcUi();

	void WorldMove();

	void Zoom();

	void WindowUpdate();

	void Erase();

	void Place();

	void Render();

private:
	float yPlace;
	float xPlace;
	float dt;
	float uiOffset;
	int selected;
	bool uiClick;
	bool mouseOver;

	int TILE_NUM;

	Camera2D cam;

	Color pixel;

	Button buttons[BUT_NUM];

	UiTile* UiTiles;

	Vector2 MousePos;

	std::vector<Tile> tileMap;
	std::vector<Tile> mesh;
};

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

	bool pause;

	float beneathPlayer;
	float dt;

	Player player;
	Vector2 spawn;

	Button buttons[1];

	std::vector<Tile> tileMap;
	std::vector<Tile> mesh;
};