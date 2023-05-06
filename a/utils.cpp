#include "utils.h"

// Mesh related functions //
void solveCollision(Player& player, const Tile& tile, float& beneathPlayer)
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
			player.setVel({ 0, player.getVel().y });
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

void CalculateMesh(std::vector<Tile> tileMap, std::vector<Tile>& mesh)
{
	//sort(tileMap.begin(), tileMap.end(), compareTiles);
	mesh.clear();

	for (const auto& tile : tileMap)
	{
		if (tile.back())
			continue;

		Vector2 pos1 = tile.getPos();

		vec2i size = Gsize(tile.getIndex());

		float xp = pos1.x + size.x;
		float xm = pos1.x - size.x;
		float yp = pos1.y + size.y;
		float ym = pos1.y - size.y;

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
			mesh.push_back(tile);
	}
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
	if (n < 0)
		return -1;
	else
		return 1;
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