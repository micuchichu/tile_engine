#include "tile.h"

// Check If Clicked //
bool Tile::Clicked(Vector2 MousePos)
{
	if (MousePos.x < pos.x + Gsize(texture_index).x && MousePos.x > pos.x
		&& MousePos.y < pos.y + Gsize(texture_index).y && MousePos.y > pos.y)
		return IsMouseButtonDown(MOUSE_BUTTON_LEFT);
	return false;
}

bool Tile::Clicked(int sizeX, int sizeY, Vector2 MousePos)
{
	if (MousePos.x < pos.x + sizeX && MousePos.x > pos.x
		&& MousePos.y < pos.y + sizeY && MousePos.y > pos.y)
		return IsMouseButtonDown(MOUSE_BUTTON_LEFT);
	return false;
}

// Check If Clicked with additional bool parameter i dont know why, never used this, i hate myself //
bool Tile::Clicked(int sizeX, int sizeY, Vector2 MousePos, bool& mouseOver)
{
	if (MousePos.x < pos.x + sizeX && MousePos.x > pos.x
		&& MousePos.y < pos.y + sizeY && MousePos.y > pos.y) {
		mouseOver = true;
		return IsMouseButtonDown(MOUSE_BUTTON_LEFT);
	}
	return false;
}

// Check If Mouse is Over the Tile //
bool Tile::MouseOver(int sizeX, int sizeY, Vector2 MousePos)
{
	if (MousePos.x < pos.x + sizeX && MousePos.x > pos.x
		&& MousePos.y < pos.y + sizeY && MousePos.y > pos.y)
		return true;
	return false;
}

// Index // 
const int Tile::getIndex() const
{
	return texture_index;
}

void Tile::setIndex(int index)
{
	texture_index = index;
}

// Position //
const Vector2 Tile::getPos() const
{
	return pos;
}

const bool Tile::back() const
{
	return textures.tiles[texture_index].back;
}

void Tile::DrawTile()
{
	DrawTexture(Gtexture(texture_index), pos.x, pos.y, RAYWHITE);
}

void Tile::DrawTile(float scale)
{
	DrawTextureEx(Gtexture(texture_index), pos, 0, scale, RAYWHITE);
}