#include <fstream>
#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>
#include "raylib.h"

#define BUILDER 0

#define TILE_NUM 10
#define BUT_NUM 3

Vector2 mult(Vector2 a, Vector2 b)
{
	return { a.x * b.x, a.y * b.y };
}

Vector2 scale(Vector2 a, float b)
{
	return { a.x * b, a.y * b };
}

Vector2 sub(Vector2 a, Vector2 b)
{
	return { a.x - b.x, a.y - b.y };
}

struct Textures
{
	void initTextures()
	{
		for (int i = 0; i < TILE_NUM; i++)
		{
			char num[3];
			sprintf_s(num, "%d", i);
			char file[32] = "textures/tiles/tile_";
			strcat_s(file, num);
			strcat_s(file, ".png");
			tiles[i] = LoadTexture(file);
		}
	}

	Texture2D tiles[TILE_NUM];
}textures;

class Button
{
public:
	Button() = default;

	Button(Texture2D texture)
		: texture(texture) {}

	Button(float x, float y)
		: pos({ x, y }) {}

	Button(std::string text, std::function<void()> func) : function_(func) {}

	Texture2D getTexture()
	{
		return texture;
	}

	void setPos(int x, int y)
	{
		pos = { (float)x, (float)y };
	}

	void setFunc(std::function<void()> func) {
		function_ = std::bind(func);
	}

	void DrawButton()
	{
		DrawTexture(texture, pos.x, pos.y, RAYWHITE);
	}

	void Clicked(Vector2 MousePos)
	{
		if (MousePos.x < pos.x + texture.width && MousePos.x > pos.x &&
			MousePos.y < pos.y + texture.height && MousePos.y > pos.y)
			if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
				function_();
	}

private:
	Texture2D texture;
	Vector2 pos;
	std::function<void()> function_;
}buttons[BUT_NUM];

class UiTile
{
public:
	UiTile() = default;

	UiTile(int index)
		: texture_index(index)
	{}

	bool Clicked(int x, int y, int size)
	{
		if (GetMousePosition().x < x + size && GetMousePosition().x > x
			&& GetMousePosition().y < y + size && GetMousePosition().y > y)
			return IsMouseButtonDown(MOUSE_BUTTON_LEFT);
		return false;
	}

	bool MouseOver(int x, int y, int size)
	{
		if (GetMousePosition().x < x + size && GetMousePosition().x > x
			&& GetMousePosition().y < y + size && GetMousePosition().y > y)
			return true;
		return false;
	}

	const int getIndex() const
	{
		return texture_index;
	}

	void DrawTile(int x, int y)
	{
		DrawTexture(textures.tiles[texture_index], x, y, RAYWHITE);
	}

	void DrawTile(float x, float y, float scale)
	{
		DrawTextureEx(textures.tiles[texture_index], { x, y }, 0, scale, RAYWHITE);
	}

private:
	int texture_index;
};

class Tile
{
public:
	Tile(int index, Vector2 pos)
		: texture_index(index), pos(pos)
	{}
	
	bool Clicked(int sizeX, int sizeY, Vector2 MousePos)
	{
		if (MousePos.x < pos.x + sizeX && MousePos.x > pos.x
			&& MousePos.y < pos.y + sizeY && MousePos.y > pos.y)
			return IsMouseButtonDown(MOUSE_BUTTON_LEFT);
		return false;
	}

	bool Clicked(int sizeX, int sizeY, Vector2 MousePos, bool& mouseOver)
	{
		if (MousePos.x < pos.x + sizeX && MousePos.x > pos.x
			&& MousePos.y < pos.y + sizeY && MousePos.y > pos.y) {
			mouseOver = true;
			return IsMouseButtonDown(MOUSE_BUTTON_LEFT);
		}
		return false;
	}

	bool MouseOver(int sizeX, int sizeY, Vector2 MousePos)
	{
		if (MousePos.x < pos.x + sizeX && MousePos.x > pos.x
			&& MousePos.y < pos.y + sizeY && MousePos.y > pos.y)
			return true;
		return false;
	}

	const int getIndex() const
	{
		return texture_index;
	}

	void setIndex(int index) 
	{
		texture_index = index;
	}

	const Vector2 getPos() const
	{
		return pos;
	}

	void DrawTile()
	{
		DrawTexture(textures.tiles[texture_index], pos.x, pos.y, RAYWHITE);
	}

	void DrawTile(float scale)
	{
		DrawTextureEx(textures.tiles[texture_index], pos, 0, scale, RAYWHITE);
	}

private:
	Vector2 pos;
	int texture_index;
};

class Player
{
public:
	Player() 
	{
		pos = { 0, 0 };
		vel = { 0, 0 };
		cam.offset = { 0, 0 };
		cam.rotation = 0;
		cam.target = { 0, 0 };
		cam.zoom = 1;
	}

	void ApplyForce(Vector2 f, float dt)
	{
		vel.x += f.x * dt;
		vel.y += f.y * dt;
	}

	void Update(float dt)
	{
		pos.x += vel.x * dt;
		pos.y += vel.y * dt;
		cam.target = pos;
	}

	const Camera2D getCam() const { return cam; }

	const Vector2 getPos() const { return pos; }

	const Vector2 getVel() const { return vel; }

	void setPos(Vector2 pos) { this->pos = pos; }
	void setVel(Vector2 vel) { this->vel = vel; }

private:
	Vector2 pos;
	Vector2 vel;
	Camera2D cam;
};

bool compareTiles(const Tile& a, const Tile& b) 
{
	return (a.getPos().y < b.getPos().y) || (a.getPos().y == b.getPos().y && a.getPos().x < b.getPos().x);
}

bool compareVectors(const Vector2& a, const Vector2& b) {
	return (a.x < b.x) || (a.x == b.x && a.y < b.y);
}

void CalculateMesh(std::vector<Tile> tileMap, std::vector<Vector2> &mesh)
{
	sort(tileMap.begin(), tileMap.end(), compareTiles);
	mesh.clear();
	for (const auto& tile : tileMap) 
	{
		float xp = tile.getPos().x + 64;
		float xm = tile.getPos().x - 64;
		float yp = tile.getPos().y + 64;
		float ym = tile.getPos().y - 64;

		int adjacent = 0;

		for (const auto& tile2 : tileMap)
		{
			if(tile.getPos().x != tile2.getPos().x && tile.getPos().y != tile2.getPos().y)
				if ((tile2.getPos().x == xp && tile2.getPos().y == yp) ||
					(tile2.getPos().x == xp && tile2.getPos().y == ym) ||
					(tile2.getPos().x == xm && tile2.getPos().y == yp) ||
					(tile2.getPos().x == xm && tile2.getPos().y == ym))
				{
					adjacent++;
				}
		}
		if (adjacent < 4 && (tile.getIndex() != 4 && tile.getIndex() != 7 && tile.getIndex() != 8 && tile.getIndex() != 9))
			mesh.push_back(tile.getPos());
	}
	//sort(mesh.begin(), mesh.end(), compareVectors);
}

void Export(std::vector<Tile>& tiles)
{
	std::ofstream file("save.txt");
	for (int i = 0; i < tiles.size(); i++)
	{
		for (int j = i + 1; j < tiles.size(); j++)
		{
			if (tiles[i].getPos().x == tiles[j].getPos().x && tiles[i].getPos().y == tiles[j].getPos().y) {
				tiles.erase(tiles.begin() + i);
			}
		}
		file << tiles[i].getIndex() << " " << tiles[i].getPos().x << " " << tiles[i].getPos().y << std::endl;
	}
}

void Import(std::vector<Tile>& tiles)
{
	std::ifstream file("save.txt");
	tiles.clear();
	while(!file.eof())
	{
		int index, x, y;
		file >> index >> x >> y;
		Tile tile(index, Vector2{ (float)x, (float)y });
		tiles.push_back(tile);
	}
}

void Delete(std::vector<Tile>& tiles)
{
	if(tiles.size())
		tiles.clear();
}

int main()
{
	if (BUILDER)
	{
		// WINDOW //
		SetWindowState(FLAG_WINDOW_RESIZABLE);
		InitWindow(800, 600, "tile engine");

		// TEXTURES //
		textures.initTextures();
		buttons[0] = LoadTexture("textures/buttons/button_0.png");
		buttons[1] = LoadTexture("textures/buttons/button_1.png");
		//buttons[2] = LoadTexture("textures/buttons/button_2.png");
		//buttons[2] = LoadTexture("buttons/button_2.png");

		// CAM //
		Camera2D cam;
		cam.offset = { 0, 0 };
		cam.rotation = 0;
		cam.target = { 0, 0, };
		cam.zoom = 1;

		// MESH //
		std::vector<Vector2> mesh;

		// TILES  //
		std::vector<Tile> tileMap;
		UiTile UiTiles[TILE_NUM];
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

			// UPDATE ON RESIZE //
			/*if (IsWindowResized())
			{
				cam.offset.x = GetScreenWidth() / 2;
				cam.offset.y = GetScreenHeight() / 2;
			}*/

			// UI //
			for (int i = 0; i < TILE_NUM; i++) {
				if (UiTiles[i].Clicked(GetScreenWidth() - 56, i * 64 + 16, 48)) {
					selected = UiTiles[i].getIndex();
					uiClick = true;
					break;
				}
				else
					uiClick = false;
			}

			// WORLD MOVE //
			if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
				cam.target.x -= GetMouseDelta().x;
				cam.target.y -= GetMouseDelta().y;
			}

			// ZOOM //
			if (GetMouseWheelMove())
			{
				cam.zoom += GetMouseWheelMove() * dt;
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

			for (int i = 0; i < tileMap.size(); i++) {
				tileMap[i].DrawTile();
				/*if (tileMap[i].MouseOver(textures.tiles[selected].width, textures.tiles[selected].height, MousePos))
					mouseOver = true;*/
			}

			for (int i = 1; i < mesh.size(); i++)
				DrawLine(mesh[i - 1].x, mesh[i - 1].y, mesh[i].x, mesh[i].y, GREEN);

			DrawTexture(textures.tiles[selected], xPlace, yPlace, { 245, 245, 245, 120 });

			EndMode2D();

			// UI //
			DrawRectangle(GetScreenWidth() - 64, 0, 64, GetScreenHeight(), DARKGRAY);
			DrawRectangle(0, 0, GetScreenWidth(), 48, DARKGRAY);

			for (int i = 0; i < TILE_NUM; i++) {
				UiTiles[i].DrawTile(GetScreenWidth() - 56, i * 64 + 16, 0.75f);

				if (UiTiles[i].MouseOver(GetScreenWidth() - 56, i * 64 + 16, 48))
					DrawRectangleLines(GetScreenWidth() - 56, i * 64 + 16, textures.tiles[i].width * 0.75f, textures.tiles[i].height * 0.75f, WHITE);
				if (UiTiles[i].getIndex() == selected)
					DrawRectangleLines(GetScreenWidth() - 56, i * 64 + 16, textures.tiles[i].width * 0.75f, textures.tiles[i].height * 0.75f, GREEN);
			}

			for (int i = 0; i < BUT_NUM; i++)
			{
				buttons[i].DrawButton();
			}

			EndDrawing();
		}

		return 0;
	}
	else
	{
		SetWindowState(FLAG_WINDOW_RESIZABLE);
		InitWindow(600, 600, "gam");

		int screenW = GetScreenWidth();
		int screenH = GetScreenHeight();

		textures.initTextures();

		std::vector<Tile> tileMap;
		std::vector<Vector2> mesh;

		Player player;

		Import(tileMap);

		CalculateMesh(tileMap, mesh);

		while (!WindowShouldClose())
		{
			float dt = GetFrameTime();

			if (IsWindowResized())
			{
				screenW = GetScreenWidth();
				screenH = GetScreenHeight();
			}

			if (abs(player.getVel().x) < 5000) {
				if (IsKeyDown('A')) {
					player.ApplyForce({ -1000, 0 }, dt);
				}
				if (IsKeyDown('D')) {
					player.ApplyForce({ 1000, 0 }, dt);
				}
			}

			// Gravity //
			for (int i = 0; i < mesh.size(); i++) 
			{
				if (mesh[i].x > player.getPos().x - 32 && mesh[i].x < player.getPos().x + 32 && mesh[i].y < player.getPos().y + 128 && mesh[i].y > player.getPos().y)
				{
					if (player.getPos().y < mesh[i].y - 64)
						player.ApplyForce({ 0, 980 }, dt);
					// Ground //
					else {
						player.setPos({ player.getPos().x, mesh[i].y - 64});

						if (player.getVel().y > 0)
							player.setVel({ player.getVel().x, 0 });

						if (IsKeyDown(' '))
						{
							player.ApplyForce({ 0, -500 }, 1);
						}
					}
				}
			}
			
			// Friction //
			player.ApplyForce(mult(player.getVel(), Vector2{ -0.75f, 0 }), dt);

			player.Update(dt);

			BeginDrawing();
				ClearBackground(BLACK);
				BeginMode2D(player.getCam());
					for (int i = 0; i < tileMap.size(); i++)
						tileMap[i].DrawTile();
				EndMode2D();
				DrawRectangle(screenW / 2, screenH / 2, 64, 128, RED);
				DrawCircle(screenW / 2, screenH / 2, 10, BLUE);
			EndDrawing();
		}
	}
}