#pragma once

#include "headers.h"

namespace fs = std::filesystem;

struct Textures
{
    void initTextures()
    {
        std::string mapping_path = "./textures/tile_mapping.txt";
        std::ifstream mapping_file(mapping_path);
        std::map<std::string, int> tile_mapping;

        // Read tile mapping from file //
        std::string line;
        int index = 0;
        while (std::getline(mapping_file, line))
        {
            tile_mapping[line] = index;
            index++;
        }

        // Load textures using the tile mapping //
        for (auto& [tile_name, tile_index] : tile_mapping)
        {
            std::string pth = "./textures/tiles/" + tile_name + ".png";
            tiles[tile_index] = LoadTexture(pth.c_str());
            std::cout << pth << std::endl;
        }
    }

    std::map<int, Texture2D> tiles;
};