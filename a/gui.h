#pragma once
#include "headers.h"
#include "textures.h"
#include "globals.h"

class Button
{
public:
	Button() = default;

	Button(Texture2D texture)
		: texture(texture) {}

	Button(float x, float y)
		: pos({ x, y }) {}

	Button(std::string text, std::function<void()> func) 
		: function_(func) {}

	Texture2D getTexture();

	void setPos(int x, int y);

	void setFunc(std::function<void()> func);

	void DrawButton();

	void DrawButton(float size);

	void Clicked(Vector2 MousePos);

	void Clicked();

	void Unload();

private:
	Texture2D texture;
	Vector2 pos;
	std::function<void()> function_;
};

class UiTile
{
public:
	UiTile() = default;

	UiTile(int index)
		: texture_index(index) {}

	bool Clicked(int x, int y, int size);

	bool MouseOver(int x, int y, int size);

	bool Clicked(int x, int y, vec2i size);

	bool MouseOver(int x, int y, vec2i size);

	const int getIndex() const;

	void DrawTile(int x, int y);

	void DrawTile(float x, float y, float scale);

private:
	int texture_index;
};