#pragma once
#include "rapidjson/document.h"
#include "headers.h"

namespace fs = std::filesystem;
namespace json = rapidjson;

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
        std::string path = "./properties/";
        for (const auto& entry : fs::directory_iterator(path))
        {
            std::ifstream file(entry.path());

            std::string file_contents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

            json::Document doc;
            doc.Parse(file_contents.c_str());

            uint64_t index;
            std::string texture;
            bool back = 0;
            float size = 1;
            
            index = doc["id"].GetInt();
            std::cout << index << std::endl;
            texture = doc["texture_path"].GetString();
            back = doc["background"].GetBool();
            size = doc["size"].GetFloat();
            
            tiles[index].texture = LoadTexture(texture.c_str());
            tiles[index].back = back;
            tiles[index].size.x = size * tiles[index].texture.width;
            tiles[index].size.y = size * tiles[index].texture.height;

            Image img = LoadImageFromTexture(tiles[index].texture);
            ImageResize(&img, tiles[index].size.x, tiles[index].size.y);
            tiles[index].texture = LoadTextureFromImage(img);
            UnloadImage(img);
        }
    }

    // Reload //
    void reload()
    {
        unload();
        initTextures();
    }

    // Unload //
    void unload()
    {
        for (uint64_t i = 0; i < tiles.size(); i++)
            UnloadTexture(tiles[i].texture);
    }

    std::map<uint64_t, item> tiles;
};