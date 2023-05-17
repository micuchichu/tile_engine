#pragma once

#include "tile.h"
#include "headers.h"
#include "utils.h"

class Chunk
{
public:
	Chunk() = default;

	std::vector<Tile> getChunk();

	void updateChunk(std::vector<Tile> newChunk);

	void push_back(Tile tile);

	size_t size();

	void clear();

	bool empty();

	void erase(std::vector<Tile>::const_iterator _Where);

	std::vector<Tile>::iterator begin();

	int firstCol();

	int lastCol();

	int middleCol();

	void setFirst(int _first);

	void setLast(int _last);

	void setMiddle(int _middle);

private:
	std::vector<Tile> chunk;
	int first;
	int last;
	int middle;
};