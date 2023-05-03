#pragma once

#include "headers.h"

namespace fs = std::filesystem;

struct item
{
    Texture2D texture;
    bool back;
    int size;
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
            tiles[index].size = size * tiles[index].texture.width;
            Image img = LoadImageFromTexture(tiles[index].texture);
            ImageResize(&img, size * tiles[index].texture.width, size * tiles[index].texture.height);
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