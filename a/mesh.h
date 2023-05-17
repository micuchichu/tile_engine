#pragma once

#include "tile.h"

class MeshObj
{
public:
	MeshObj() = default;

	MeshObj(Tile tile, int index, int chunk)
		: _tile(tile), chunk_index(index), _chunk(chunk) {};

	Tile tile();

	int chunkIndex();

	int chunk();

private:
	Tile _tile;
	int chunk_index;
	int _chunk;
};