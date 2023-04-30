#include <fstream>
#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>
#include <cmath>
#include <filesystem>
#include "raylib.h"

//namespace fs = filesystem;

#define KEY_INVALID 163

#define TILE_NUM 11
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

float length(Vector2 a)
{
	return sqrt(a.x * a.x + a.y * a.y);
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

			//for (const auto& entry : filesystem::directory_iterator(path)) {

			//	// Converting the path to const char * in the
			//	// subsequent lines
			//	std::filesystem::path outfilename = entry.path();
			//	std::string outfilename_str = outfilename.string();
			//	const char* path = outfilename_str.c_str();

			//	// Testing whether the path points to a
			//	// non-directory or not If it does, displays path
			//	if (stat(path, &sb) == 0 && !(sb.st_mode & S_IFDIR))
			//		std::cout << path << std::endl;
			//}
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

	void DrawButton(float size)
	{
		DrawTextureEx(texture, pos, 0, size, RAYWHITE);
	}

	void Clicked(Vector2 MousePos)
	{
		if (MousePos.x < pos.x + texture.width && MousePos.x > pos.x &&
			MousePos.y < pos.y + texture.height && MousePos.y > pos.y)
			if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
				function_();
	}

	void Clicked()
	{
		Clicked(GetMousePosition());
	}

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

	const bool back() const
	{
		if (texture_index == 7 || texture_index == 8 || texture_index == 9)
			return true;
		else
			return false;
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

	void setCamOffset(Vector2 offset) { this->cam.offset = offset; }

	void setPos(Vector2 pos) { this->pos = pos; }
	void setX(float x) { this->pos.x = x; }
	void setY(float y) { this->pos.y = y; }

	void setVel(Vector2 vel) { this->vel = vel; }

	void initTexture() { texture = LoadTexture("./textures/player/player2.png"); }

	Texture2D getTexture() { return texture; }

	void Draw()
	{
		DrawTextureEx(texture, pos, 0, 1, RAYWHITE);
	}
private:
	Vector2 pos;
	Vector2 vel;
	Camera2D cam;
	Texture2D texture;
};

bool compareTiles(const Tile& a, const Tile& b) 
{
	return (a.getPos().y < b.getPos().y) || (a.getPos().y == b.getPos().y && a.getPos().x < b.getPos().x);
}

bool compareVectors(const Vector2& a, const Vector2& b) {
	return (a.x < b.x) || (a.x == b.x && a.y < b.y);
}

void solveCollision(Player& player, const Vector2& squarePos, float& beneathPlayer) 
{
	const float squareSize = 64.0f;
	const float playerWidth = player.getTexture().width;
	const float playerHeight = player.getTexture().height;

	float squareLeft = squarePos.x;
	float squareRight = squarePos.x + squareSize;
	float squareTop = squarePos.y;
	float squareBottom = squarePos.y + squareSize;

	Vector2 playerPos = player.getPos();

	if (playerPos.x < squareRight && playerPos.x + playerWidth > squareLeft &&
		playerPos.y < squareBottom && playerPos.y + playerHeight > squareTop) 
	{
		float dxLeft = std::abs(squareLeft - (playerPos.x + playerWidth));
		float dxRight = std::abs(squareRight - playerPos.x);
		float dyTop = std::abs(squareTop - (playerPos.y + playerHeight));
		float dyBottom = std::abs(squareBottom - playerPos.y);

		float minDistance = std::min({ dxLeft, dxRight, dyTop, dyBottom });

		if (minDistance == dxLeft) 
		{
			player.setPos({ squareLeft - playerWidth, playerPos.y });
			player.setVel({ 0, player.getVel().y });
		}
		else if (minDistance == dxRight)
		{
			player.setPos({ squareRight, playerPos.y });
			player.setVel({ 0, player.getVel().y});
		}
		else if (minDistance == dyTop) 
		{
			beneathPlayer = squarePos.y - playerHeight;
		}
		else 
		{
			player.setPos({ playerPos.x, squareBottom });
			player.setVel({ player.getVel().x, 0 });
		}
	}
}

void CalculateMesh(std::vector<Tile> tileMap, std::vector<Vector2> &mesh) 
{
	//sort(tileMap.begin(), tileMap.end(), compareTiles);
	mesh.clear();

	for (const auto& tile : tileMap) 
	{
		if (tile.back())
			continue;

		Vector2 pos1 = tile.getPos();

		float xp = pos1.x + 64;
		float xm = pos1.x - 64;
		float yp = pos1.y + 64;
		float ym = pos1.y - 64;

		int adjacent = 0;

		for (const auto& tile2 : tileMap)
		{
			if (tile2.back())
				continue;

			Vector2 pos2 = tile2.getPos();

			if ((pos2.x == pos1.x && pos2.y == ym) ||
				(pos2.x == xp && pos2.y == pos1.y) ||
				(pos2.x == pos1.x && pos2.y == yp) ||
				(pos2.x == xm && pos2.y == pos1.y))
			{
				adjacent++;
			}

			if (adjacent == 4)
				break;
		}
		if (adjacent < 4)
			mesh.push_back(tile.getPos());
	}
}

float Distance(Vector2 a, Vector2 b)
{
	return length(sub(a, b));
}

void Export(std::vector<Tile>& tiles)
{
	std::ofstream file("./saves/save.txt");
	for (int i = 0; i < tiles.size(); i++)
	{
		for (int j = 0; j < tiles.size(); j++)
		{
			if (i != j)
				if (tiles[i].getPos().x == tiles[j].getPos().x && tiles[i].getPos().y == tiles[j].getPos().y) {
					tiles.erase(tiles.begin() + j);
				}
		}
		file << tiles[i].getIndex() << " " << tiles[i].getPos().x << " " << tiles[i].getPos().y << std::endl;

	}
}

void Import(std::vector<Tile>& tiles)
{
	std::ifstream file("./saves/save.txt");
	tiles.clear();

	while(!file.eof())
	{
		int index, x, y;
		file >> index >> x >> y;
		Tile tile(index, Vector2{ (float)x, (float)y });
		tiles.push_back(tile);
	}

	for (int i = 0; i < tiles.size(); i++)
	{
		for (int j = 0; j < tiles.size(); j++)
		{
			if (i != j)
				if (tiles[i].getPos().x == tiles[j].getPos().x && tiles[i].getPos().y == tiles[j].getPos().y) {
					tiles.erase(tiles.begin() + j);
				}
		}
	}
}

void Delete(std::vector<Tile>& tiles)
{
	if(tiles.size())
		tiles.clear();
}

int main()
{
	bool builder;

	std::cout << "Editor (1/0): ";
	std::cin >> builder;

	// Level Editor //
	if (builder)
	{
		// WINDOW //
		SetWindowState(FLAG_WINDOW_RESIZABLE);
		InitWindow(800, 600, "tile engine");
		SetExitKey(KEY_INVALID);

		// BUTTONS //
		Button buttons[BUT_NUM];

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

			for (int i = 0; i < mesh.size(); i++)
				DrawRectangleLines(mesh[i].x, mesh[i].y, 64, 64, GREEN);

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
	
	// Game Runner //
	else
	{
		// Init Window //
		SetWindowState(FLAG_WINDOW_RESIZABLE);
		InitWindow(600, 600, "gam");
		SetExitKey(KEY_INVALID);

		// Screen Bounds //
		int screenW = GetScreenWidth();
		int screenH = GetScreenHeight();
		bool pause = true;

		// Initiate Player //
		Player player;
		player.setCamOffset({ (float)screenW / 2, (float)screenH / 2 });

		// Load Textures //
		player.initTexture();
		textures.initTextures();

		// World and Mesh //
		std::vector<Tile> tileMap;
		std::vector<Vector2> mesh;

		// Load World //
		Import(tileMap);

		// Spawn //
		Vector2 spawn = {0, 0};
		for (auto tile : tileMap)
			if (tile.getIndex() == 10)
				spawn = {tile.getPos().x, tile.getPos().y - player.getTexture().height};
		player.setPos(spawn);

		// Buttons //
		Button buttons[1];
		buttons[0].setPos(10, 10);
		buttons[0] = LoadTexture("./textures/buttons/respawn_button.png");

		auto lambda1 = [&spawn, &player]() { player.setPos(spawn); };
		std::function<void()> func1 = lambda1;
		buttons[0].setFunc(func1);

		// Calculate Mesh //
		CalculateMesh(tileMap, mesh);

		// Game Loop //
		while (!WindowShouldClose())
		{
			// Delta Time //
			float dt = GetFrameTime();
			
			// Window Resized //
			if (IsWindowResized())
			{
				pause = true;
				screenW = GetScreenWidth();
				screenH = GetScreenHeight();
				player.setCamOffset({(float)screenW / 2, (float)screenH / 2});
			}

			// Pause //
			if (IsKeyPressed(KEY_ESCAPE))
				pause = !pause;

			// Buttons //
			for (int i = 0; i < 1; i++)
				buttons[i].Clicked(GetMousePosition());

			if (!pause)
			{
				float beneathPlayer = 704;

				// Solve Collisions //
				for (int i = 0; i < mesh.size(); i++)
					solveCollision(player, mesh[i], beneathPlayer);

				// Movement //
				if (abs(player.getVel().x) < 5000)
				{
					if (IsKeyDown('A'))
						player.ApplyForce({ -1000, 0 }, dt);

					if (IsKeyDown('D'))
						player.ApplyForce({ 1000, 0 }, dt);
				}

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

				// Friction //
				player.ApplyForce(mult(player.getVel(), Vector2{ -0.75f, 0 }), dt);

				// Update //
				player.Update(dt);
			}

			if (IsKeyPressed('F'))
				std::cout << GetFPS() << std::endl;

			// Render //
			BeginDrawing();
				ClearBackground(BLACK);
				BeginMode2D(player.getCam());
					// Draw Tiles //
					for (int i = 0; i < tileMap.size(); i++)
						tileMap[i].DrawTile();

					// Draw Player //
					player.Draw();
					
					// Draw Mesh //
					//for (int i = 0; i < mesh.size(); i++)
					//	DrawRectangleLines(mesh[i].x, mesh[i].y, 64, 64, GREEN);
				EndMode2D();

				buttons[0].DrawButton(0.5f);
			
				if (pause)
					DrawText("Game Paused \nPress ESC to Unpause", screenW / 2, 50, 20, WHITE);

			EndDrawing();
		}
	}
}