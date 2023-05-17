#include "world.h"

void LoadChunk(std::map<int, Chunk>& chunks, int chunk)
{
    if(chunks.find(chunk) != chunks.end())
        return;

    Tile tile(0, { 0, 0 });
    int index = 0;
    float y = float((int)(generateNoise(16 * chunk, 0) * 2048 / 64));

    for (int i = 16 * chunk; i < 16 + 16 * chunk; i++)
    {
        for (int j = y + 1; j < 32; j++)
        {
            if (generateNoise(i, j) < 0.5f) {
                if (j < y + 4)
                    tile = { 5, Vector2{ i * 64.0f, j * 64.0f } };
                else
                    tile = { 1, Vector2{ i * 64.0f, j * 64.0f } };

                chunks[chunk].push_back(tile);

                if(i == 16 * chunk + 8)
                    chunks[chunk].setMiddle(index);
            }

            index++;
        }

        tile = { 3, Vector2{ i * 64.0f, y * 64.0f } };
        chunks[chunk].push_back(tile);
        index++;

        y = float((int)(generateNoise(i + 1, 0) * 2048 / 64));
    }
}

void UnloadChunk(std::map<int, Chunk>& tiles, int chunk)
{
	
}
