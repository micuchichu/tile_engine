#include "perlin.h"

void permutations(int seed)
{
    srand(seed);

    for (int i = 0; i < PERMS; i++)
    {
        p[i] = rand() % PERMS;
    }
}

float fade(float t) 
{
    return t * t * t * (t * (t * 6 - 15) + 10);
}

float lerp(float t, float a, float b) 
{
    return a + t * (b - a);
}

float grad(int hash, float x, float y, float z) 
{
    int h = hash & 15;
    float u = h < 8 ? x : y;
    float v = h < 4 ? y : h == 12 || h == 14 ? x : z;
    return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

float noise(float x, float y, float z) 
{
    int X = (int)floor(x) & (PERMS - 1);
    int Y = (int)floor(y) & (PERMS - 1);
    int Z = (int)floor(z) & (PERMS - 1);

    x -= floor(x);
    y -= floor(y);
    z -= floor(z);

    float u = fade(x);
    float v = fade(y);
    float w = fade(z);

    int A = p[X % PERMS] + Y;
    int AA = p[A % PERMS] + Z;
    int AB = p[(A + 1) % PERMS] + Z;
    int B = p[(X + 1) % PERMS] + Y;
    int BA = p[B % PERMS] + Z;
    int BB = p[(B + 1) % PERMS] + Z;

    return lerp(w,
        lerp(v,
            lerp(u,
                grad(p[AA], x, y, z),
                grad(p[BA], x - 1, y, z)),
            lerp(u, grad(p[AB], x, y - 1, z),
                grad(p[BB], x - 1, y - 1, z))),
        lerp(v,
            lerp(u,
                grad(p[AA + 1], x, y, z - 1),
                grad(p[BA + 1], x - 1, y, z - 1)),
            lerp(u, grad(p[AB + 1], x, y - 1, z - 1),
                grad(p[BB + 1], x - 1, y - 1, z - 1))));
}

float generateNoise(float x, float y)
{
    float scale = 0.1f;
    float amplitude = 1.0f;
    float persistence = 0.5f;
    float total = 0.0f;
    float frequency = 0.1f;

    for (int i = 0; i < 5; i++) {
        total += noise((x + i) * scale * frequency, (y + i) * scale * frequency, 0) * amplitude;
        amplitude *= persistence;
        frequency *= 2.0f;
    }

    return total;
}