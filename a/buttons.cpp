#include "buttons.h"

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

	while (!file.eof())
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
	if (tiles.size())
		tiles.clear();
}