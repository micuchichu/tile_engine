#pragma once

#include "headers.h"

namespace fs = std::filesystem;

struct Textures
{
    void initTextures()
    {
        std::string path = "./tiles/";
        for (const auto& entry : fs::directory_iterator(path))
        {
            std::ifstream file(entry.path());
            int index;
            std::string texture;
            bool back;
            file >> index >> texture >> back;
            tiles[index] = LoadTexture(texture.c_str());
        }
    }

    std::map<int, Texture2D> tiles;
};