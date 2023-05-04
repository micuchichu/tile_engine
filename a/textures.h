#pragma once

#include "headers.h"

namespace fs = std::filesystem;

struct vec2i
{
    int x, y;

    vec2i operator * (float n) { return { (int)(x * n), (int)(y * n) }; }
};

struct item
{
    Texture2D texture;
    bool back;
    vec2i size;
};

struct Textures
{
    // Initialize Textures //
    void initTextures()
    {
        std::string path = "./tiles/";
        for (const auto& entry : fs::directory_iterator(path))
        {
            std::ifstream file(entry.path());
            int index;
            std::string texture;
            bool back;
            float size = 1;
            file >> index >> texture >> back >> size;
            
            tiles[index].texture = LoadTexture(texture.c_str());
            tiles[index].back = back;
            tiles[index].size.x = size * tiles[index].texture.width;
            tiles[index].size.y = size * tiles[index].texture.height;

            std::cout << size << std::endl;
            std::cout << entry.path() << std::endl;

            Image img = LoadImageFromTexture(tiles[index].texture);
            ImageResize(&img, tiles[index].size.x, tiles[index].size.y);
            tiles[index].texture = LoadTextureFromImage(img);
            UnloadImage(img);
        }
    }

    // Reload //
    void reload()
    {
        for (int i = 0; i < tiles.size(); i++)
            UnloadTexture(tiles[i].texture);
        initTextures();
    }

    std::map<int, item> tiles;
};