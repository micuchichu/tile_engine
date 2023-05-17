#include "chunk.h"

std::vector<Tile> Chunk::getChunk()
{
	return chunk;
}

void Chunk::updateChunk(std::vector<Tile> newChunk)
{
	chunk = newChunk;
}

void Chunk::push_back(Tile tile)
{
	chunk.push_back(tile);
}

size_t Chunk::size()
{
	return chunk.size();
}

void Chunk::clear()
{
	chunk.clear();
}

bool Chunk::empty()
{
	return chunk.empty();
}

void Chunk::erase(std::vector<Tile>::const_iterator _Where)
{
	chunk.erase(_Where);
}

std::vector<Tile>::iterator Chunk::begin()
{
	return chunk.begin();
}

int Chunk::firstCol()
{
	return first;
}

int Chunk::lastCol()
{
	return last;
}

int Chunk::middleCol()
{
	return middle;
}

void Chunk::setFirst(int _first)
{
	first = _first;
}


void Chunk::setLast(int _last)
{
	last = _last;
}

void Chunk::setMiddle(int _middle)
{
	middle = _middle;
}