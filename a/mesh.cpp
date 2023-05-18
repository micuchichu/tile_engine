#include "mesh.h"

Tile MeshObj::tile()
{
	return _tile;
}

int MeshObj::chunkIndex()
{
	return chunk_index;
}

int MeshObj::chunk()
{
	return _chunk;
}