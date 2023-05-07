#include "modes.h"

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
		Level level;
		level.Run();
	}
}