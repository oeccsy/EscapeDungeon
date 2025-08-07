#include "Game/Game.h"

#include <iostream>

int main()
{
	std::cout << "==== 서버 시작 ====" << '\n';

	Game game;
	game.Run();
	
    return 0;
}