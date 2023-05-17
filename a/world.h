#pragma once
#include "headers.h"
#include "tile.h"
#include "perlin.h"
#include "utils.h"
#include "chunk.h"

void LoadChunk(std::map<int, Chunk>& chunks, int chunk);

void UnloadChunk(std::map<int, Chunk>& tiles, int chunk);