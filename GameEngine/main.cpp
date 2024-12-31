
#include <iostream>
#include "GameEngine.h"
int main()
{
    std::cout << "Hello World!\n";
    GameEngine gameEngine;
    gameEngine.Init(L"Test", L"Engine");
    gameEngine.Run();
    gameEngine.Quit();
}

