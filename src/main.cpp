#include <iostream>
#include <Engine.h>

#include "SkiGame.h"

int main()
{
    _ENGINE_LOG("Launcher", "Hello configured project!.")
    
    GameContent::SkiGame game = GameContent::SkiGame(800, 600, "Ski Game");
    game.Run();

    return 0;
}