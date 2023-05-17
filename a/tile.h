#pragma once
#include "headers.h"
#include "textures.h"
#include "globals.h"

class Tile
{
public:
	Tile(int index, Vector2 pos)
		: texture_index(index), pos(pos)
	{}

	bool Clicked(Vector2 MousePos);

	bool Clicked(int sizeX, int sizeY, Vector2 MousePos);

	bool Clicked(int sizeX, int sizeY, Vector2 MousePos, bool& mouseOver);

	bool MouseOver(int sizeX, int sizeY, Vector2 MousePos);

	const int getIndex() const;

	void setIndex(int index);

	const Vector2 getPos() const;

	const bool back() const;

	void DrawTile();

	void DrawTile(float scale);

private:
	Vector2 pos;
	int texture_index;
};