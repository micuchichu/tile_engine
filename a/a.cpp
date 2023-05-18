#include "editor.h"
#include "level.h"

Textures textures;

int main()
{
	bool builder;

	std::cout << "Editor (1/0): ";
	std::cin >> builder;

	if (builder)
	{
		Editor editor;
		editor.Run();
	}
	else
	{
		int seed;
		std::cin >> seed;
		permutations(seed);
		Level level;
		level.Run();
	}
}