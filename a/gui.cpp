#include "gui.h"

// Button //
Texture2D Button::getTexture()
{
	return texture;
}

void Button::setPos(int x, int y)
{
	pos = { (float)x, (float)y };
}

void Button::setFunc(std::function<void()> func) {
	function_ = std::bind(func);
}

void Button::DrawButton()
{
	DrawTexture(texture, pos.x, pos.y, RAYWHITE);
}

void Button::DrawButton(float size)
{
	DrawTextureEx(texture, pos, 0, size, RAYWHITE);
}

void Button::Clicked(Vector2 MousePos)
{
	if (MousePos.x < pos.x + texture.width && MousePos.x > pos.x &&
		MousePos.y < pos.y + texture.height && MousePos.y > pos.y)
		if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
			function_();
}

void Button::Clicked()
{
	Clicked(GetMousePosition());
}


// Ui Tile //
bool UiTile::Clicked(int x, int y, int size)
{
	if (GetMousePosition().x < x + size && GetMousePosition().x > x
		&& GetMousePosition().y < y + size && GetMousePosition().y > y)
		return IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
	return false;
}

bool UiTile::MouseOver(int x, int y, int size)
{
	if (GetMousePosition().x < x + size && GetMousePosition().x > x
		&& GetMousePosition().y < y + size && GetMousePosition().y > y)
		return true;
	return false;
}

const int UiTile::getIndex() const
{
	return UiTile::texture_index;
}

void UiTile::DrawTile(int x, int y)
{
	DrawTexture(textures.tiles[texture_index], x, y, RAYWHITE);
}

void UiTile::DrawTile(float x, float y, float scale)
{
	DrawTextureEx(textures.tiles[texture_index], { x, y }, 0, scale, RAYWHITE);
}