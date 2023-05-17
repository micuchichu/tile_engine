#include "utils.h"
#include <map>
#include "chunk.h"

// Mesh related functions //
void solveCollision(Player& player, const Tile& tile, float& ground)
{
	Vector2 squarePos = tile.getPos();
	const float squareSizeX = Gsize(tile.getIndex()).x;
	const float squareSizeY = Gsize(tile.getIndex()).y;
	const float playerWidth = player.getWidth();
	const float playerHeight = player.getTexture().height;

	float squareLeft = squarePos.x;
	float squareRight = squarePos.x + squareSizeX;
	float squareTop = squarePos.y;
	float squareBottom = squarePos.y + squareSizeY;

	Vector2 playerPos = player.getPos();
	Vector2 playerVel = player.getVel();

	const float playerRight = playerPos.x + playerWidth;
	const float playerBottom = playerPos.y + playerHeight;

	if (playerPos.x < squareRight && playerRight > squareLeft &&
		playerPos.y < squareBottom && playerBottom > squareTop)
	{
		const float dxLeft = std::abs(squareLeft - playerRight);
		const float dxRight = std::abs(squareRight - playerPos.x);
		const float dyTop = std::abs(squareTop - playerBottom);
		const float dyBottom = std::abs(squareBottom - playerPos.y);

		const float minDistance = std::min({ dxLeft, dxRight, dyTop, dyBottom });

		if (minDistance == dxLeft)
		{
			playerPos.x = squareLeft - playerWidth;
			playerVel.x = 0;
		}
		else if (minDistance == dxRight)
		{
			playerPos.x = squareRight;
			playerVel.x = 0;
		}
		else if (minDistance == dyTop)
		{
			ground = squarePos.y - playerHeight;
			// tile functions //

			// ajutor //
			return;
		}
		else
		{
			playerPos.y = squareBottom;
			playerVel.y = 0;
		}
	}

	player.setPos(playerPos);
	player.setVel(playerVel);
}

struct _tile
{
	_tile(Tile t, int c, int i)
		: tile(t), chunk(c), index(i) {}

	Tile tile;
	int chunk;
	int index;
};

void CalculateMesh(std::map<int, Chunk>& tileMap, std::vector<MeshObj>& mesh, int chunk)
{
	mesh.clear();
	//mesh.reserve(tileMap[chunk].getChunk().size());

	// chunk //
	std::vector<_tile> newMap;
	/*newMap.reserve(tileMap[chunk].size());*/

	if(tileMap[chunk].size())
		for(int i = 0; i < tileMap[chunk].size(); i++)
			 newMap.push_back({tileMap[chunk].getChunk()[i], chunk, i });

	// right strip //
	if(tileMap[chunk + 1].size() > 0)
		for (int i = 0; i <= tileMap[chunk + 1].middleCol(); i++)
			newMap.push_back(_tile{ tileMap[chunk + 1].getChunk()[i], chunk + 1, i });

	// left strip //
	if (tileMap[chunk - 1].size() > 0)
		for (int i = tileMap[chunk - 1].middleCol(); i <= tileMap[chunk - 1].size(); i++)
			newMap.push_back(_tile{ tileMap[chunk - 1].getChunk()[i], chunk - 1, i });

	for (const auto& tile : newMap)
	{
		if (tile.tile.back())
			continue;

		const Vector2& pos1 = tile.tile.getPos();
		const vec2i size = Gsize(tile.tile.getIndex());

		const float xp = pos1.x + size.x;
		const float xm = pos1.x - size.x;
		const float yp = pos1.y + size.y;
		const float ym = pos1.y - size.y;

		int adjacent = 0;
		
		for (const auto& tile2 : newMap)
		{
			if (tile2.tile.back())
				continue;

			const Vector2& pos2 = tile2.tile.getPos();

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
			mesh.push_back(MeshObj{ tile.tile, tile.index, tile.chunk });
	}
}


template<typename T>
T bSearch(T begin, T end, int key)
{
	T lower = begin;
	T upper = end;
	while (lower < upper)
	{
		T mid = lower + std::distance(upper, lower) / 2;
		if (key == *mid)
		{
			return mid;
		}
		if (key < *mid) {
			upper = mid;
		}
		else {
			lower = mid;
		}
	}

	return end;
}

// Vector2 Related Functions //
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

float sgn(float n)
{
	return (n < 0) ? -1 : 1;
}

Vector2 norm(Vector2 a)
{
	float length = std::sqrt(a.x * a.x + a.y * a.y);
	if (abs(length) < 1.0f)
	{
		a.x /= length;
		a.y /= length;
	}
	else
		return { sgn(a.x), sgn(a.y) };
	return a;
}

// Misc //
float Distance(Vector2 a, Vector2 b)
{
	return length(sub(a, b));
}

bool compareTiles(const Tile& a, const Tile& b)
{
	return (a.getPos().y < b.getPos().y) || (a.getPos().y == b.getPos().y && a.getPos().x < b.getPos().x);
}

bool compareVectors(const Vector2& a, const Vector2& b) {
	return (a.x < b.x) || (a.x == b.x && a.y < b.y);
}

void Window()
{
	Image icon = LoadImage("./textures/icon.png");
	SetWindowIcon(icon);
	SetWindowState(FLAG_WINDOW_RESIZABLE);
	InitWindow(600, 600, "gam");
	SetExitKey(KEY_INVALID);
};