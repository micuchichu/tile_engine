#pragma once

#include "player.h"
#include "headers.h"
#include "tile.h"
#include "mesh.h"

#define KEY_INVALID 163

// Mesh related functions //
void solveCollision(Player& player, const Tile& tile, float& ground);

void CalculateMesh(std::map<int, class Chunk>& tileMap, std::vector<class MeshObj>& mesh, int chunk);

template<typename T>
T bSearch(T begin, T end, int key);

// Vector2 Related Functions //
Vector2 mult(Vector2 a, Vector2 b);

Vector2 scale(Vector2 a, float b);

Vector2 sub(Vector2 a, Vector2 b);

float length(Vector2 a);

float sgn(float n);

Vector2 norm(Vector2 a);

// Misc //
float Distance(Vector2 a, Vector2 b);

bool compareTiles(const Tile& a, const Tile& b);

bool compareVectors(const Vector2& a, const Vector2& b);

void Window();