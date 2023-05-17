#pragma once

#include "headers.h"
#include "textures.h"
#include "tile.h"
#include "utils.h"
#include "gui.h"
#include "globals.h"
#include "buttons.h"
#include "mesh.h"

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

	void KeysPressed();

	void Buttons();

	void CalcUi();

	void WorldMove();

	void Zoom();

	void WindowUpdate();

	void Erase();

	void Place();

	void Render();

	void Unload();

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
	std::vector<MeshObj> mesh;
};
