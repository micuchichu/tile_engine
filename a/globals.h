#pragma once
#include "textures.h"

extern Textures textures;

#define Gtexture(x) textures.tiles[x].texture
#define Gsize(x) textures.tiles[x].size